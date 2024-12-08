#ifndef DNS_RESOLVER_H
#define DNS_RESOLVER_H

#include <string>
#include <memory>
#include <vector>
#include <uv.h>


void outputResult(const std::string &ip, const std::string &result, bool useJson, bool useXml);


class DnsRequest
{
public:
    std::string ip;
    std::string dns_server;

    DnsRequest(const std::string &ip, const std::string &dns_server);
};


class DnsResolver
{
private:
    static bool useJson;
    static bool useXml;

public:
    static void setUseJson(bool value);
    static void setUseXml(bool value);
    static bool getUseJson();
    static bool getUseXml();
    static void onDnsResolvedWrapper(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
    static void onDnsResolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res, std::ostream &output);
    static void asyncReverseDnsLookup(
        const std::string &ip,
        const std::string &dns_server,
        uv_loop_t *const loop,
        bool useJson,
        bool useXml);
    static void reverseDnsScannerAsync(
        const std::string &ips,
        const std::string &dns_server,
        uv_loop_t *const loop,
        bool useJson,
        bool useXml);
};

#endif