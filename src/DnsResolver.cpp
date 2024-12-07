#include "DnsResolver.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

#include <uv.h>

bool DnsResolver::useJson = false;
bool DnsResolver::useXml = false;

DnsRequest::DnsRequest(const std::string &ip, const std::string &dns_server)
    : ip(ip), dns_server(dns_server) {}

void DnsResolver::setUseJson(bool value)
{
    useJson = value;
}

void DnsResolver::setUseXml(bool value)
{
    useXml = value;
}

bool DnsResolver::getUseJson()
{
    return useJson;
}

bool DnsResolver::getUseXml()
{
    return useXml;
}

void DnsResolver::onDnsResolvedWrapper(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    std::ostream &output = std::cout;
    onDnsResolved(req, status, res, output);  
    uv_freeaddrinfo(res);
}

void DnsResolver::onDnsResolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res, std::ostream &output)
{
    auto dnsRequest = std::unique_ptr<DnsRequest>(reinterpret_cast<DnsRequest *>(req->data));

    if (res != nullptr && status == 0)
    {
        std::vector<char> dynamicBuffer(NI_MAXHOST);

        int result = getnameinfo(
            res->ai_addr, res->ai_addrlen,
            dynamicBuffer.data(), dynamicBuffer.size(),
            nullptr, 0, 0);

        if (result == 0)
        {
            dynamicBuffer.resize(strlen(dynamicBuffer.data()));

            outputResult(dnsRequest->ip, std::string(dynamicBuffer.data()), useJson, useXml);
        }
        else
        {
            outputResult(
                dnsRequest->ip,
                "Unable to resolve: " + std::string(gai_strerror(result)),
                useJson,
                useXml);
        }
    }
    else
    {
        outputResult(dnsRequest->ip, "Unable to resolve", useJson, useXml);
    }
}


void DnsResolver::asyncReverseDnsLookup(
    const std::string &ip,
    const std::string &dns_server,
    uv_loop_t *const loop,
    bool useJson,
    bool useXml)
{
    auto request = std::make_unique<uv_getaddrinfo_t>();
    auto dnsRequest = std::make_unique<DnsRequest>(ip, dns_server);
    request->data = dnsRequest.get();

    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int result = uv_getaddrinfo(
        loop,
        request.get(),
        DnsResolver::onDnsResolvedWrapper,
        ip.c_str(),
        nullptr,
        &hints);

    if (result != 0)
    {
        std::cerr << ip << " -> Unable to initiate DNS resolution: " << uv_strerror(result) << std::endl;
    }
    else
    {
        dnsRequest.release();
        request.release();
    }
}

void DnsResolver::reverseDnsScannerAsync(
    const std::string &ips,
    const std::string &dns_server,
    uv_loop_t *const loop,
    bool useJson,
    bool useXml)
{
    std::istringstream iss(ips);
    std::string ip;

    while (std::getline(iss, ip))
    {
        if (!ip.empty())
        {
            asyncReverseDnsLookup(ip, dns_server, loop, useJson, useXml);
        }
    }
}
