#ifndef DNS_RESOLVER_H
#define DNS_RESOLVER_H

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <uv.h>
#include <cstring>
#include <cstdlib>

class DnsRequest {
public:
    DnsRequest(const std::string &ip, const std::string &dns_server)
        : ip(ip), dns_server(dns_server) {}

    std::string ip;
    std::string dns_server;
};

class DnsResolver {
public:
    static bool useJson;
    static bool useXml;

    static void setUseJson(bool value) { useJson = value; }
    static void setUseXml(bool value) { useXml = value; }
    static bool getUseJson() { return useJson; }
    static bool getUseXml() { return useXml; }

    static void onDnsResolvedWrapper(uv_getaddrinfo_t *req, int status, struct addrinfo *res) {
        std::ostream &output = std::cout;
        onDnsResolved(req, status, res, output);  
        uv_freeaddrinfo(res);
    }

    static void onDnsResolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res, std::ostream &output) {
        auto dnsRequest = std::unique_ptr<DnsRequest>(reinterpret_cast<DnsRequest *>(req->data));

        if (res != nullptr && status == 0) {
            std::vector<char> dynamicBuffer(NI_MAXHOST);

            int result = getnameinfo(
                res->ai_addr, res->ai_addrlen,
                dynamicBuffer.data(), dynamicBuffer.size(),
                nullptr, 0, 0);

            if (result == 0) {
                dynamicBuffer.resize(strlen(dynamicBuffer.data()));
                outputResult(dnsRequest->ip, std::string(dynamicBuffer.data()), useJson, useXml);
            } else {
                outputResult(dnsRequest->ip, "Unable to resolve: " + std::string(gai_strerror(result)), useJson, useXml);
            }
        } else {
            outputResult(dnsRequest->ip, "Unable to resolve", useJson, useXml);
        }
    }

    static void asyncReverseDnsLookup(
        const std::string &ip,
        const std::string &dns_server,
        uv_loop_t *const loop,
        bool useJson,
        bool useXml) {

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

        if (result != 0) {
            std::cerr << ip << " -> Unable to initiate DNS resolution: " << uv_strerror(result) << std::endl;
        } else {
            dnsRequest.release();
            request.release();
        }
    }

    static void reverseDnsScannerAsync(
        const std::string &ips,
        const std::string &dns_server,
        uv_loop_t *const loop,
        bool useJson,
        bool useXml) {

        std::istringstream iss(ips);
        std::string ip;

        while (std::getline(iss, ip)) {
            if (!ip.empty()) {
                asyncReverseDnsLookup(ip, dns_server, loop, useJson, useXml);
            }
        }
    }

private:
static void outputResult(const std::string &ip, const std::string &result, bool useJson, bool useXml) {
    if (useJson) {
        std::cout << "{\"ip\":\"" << ip << "\",\"result\":\"" << result << "\"}" << std::endl;
    } else if (useXml) {
        std::cout << "<dns><ip>" << ip << "</ip><result>" << result << "</result></dns>" << std::endl;
    } else {
        std::cout << ip << " -> " << result << std::endl;
    }
}
};

// Static member variables definition
bool DnsResolver::useJson = false;
bool DnsResolver::useXml = false;

#endif