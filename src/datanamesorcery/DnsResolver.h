#ifndef DNS_RESOLVER_H
#define DNS_RESOLVER_H


#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>

#include <Poco/Net/DNS.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/HostEntry.h>
#include <Poco/Exception.h>


#ifdef DEBUG
#define DEBUG_OUTPUT_JSON_XML
#endif

inline void useJsonXml(bool useJson, bool useXml) {
    (void)useJson;
    (void)useXml;

#ifdef DEBUG_OUTPUT_JSON_XML
    if (useJson) {
        std::cout << "[DEBUG] JSON format is enabled." << std::endl;
    }
    if (useXml) {
        std::cout << "[DEBUG] XML format is enabled." << std::endl;
    }
#endif
}

class DnsRequest {
public:
    DnsRequest(const std::string &ip, const std::string &dns_server)
        : ip(ip), dns_server(dns_server) {}

    std::string ip;
    std::string dns_server;
};

class DnsResolver {
public:
    static inline bool useJson = false;
    static inline bool useXml = false;

    static inline void setUseJson(bool value) { useJson = value; }
    static inline void setUseXml(bool value) { useXml = value; }
    static inline bool getUseJson() { return useJson; }
    static inline bool getUseXml() { return useXml; }

    static void onDnsResolvedWrapper(std::shared_ptr<DnsRequest> dnsRequest, const Poco::Net::HostEntry &entry, const std::string &error) {
        onDnsResolved(dnsRequest, entry, error);
    }

static void onDnsResolved(std::shared_ptr<DnsRequest> dnsRequest, const Poco::Net::HostEntry &entry, const std::string &error) {
    useJsonXml(useJson, useXml);

    if (!error.empty()) {
        outputResult(dnsRequest->ip, "Unable to resolve", useJson, useXml);
        return;
    }

    if (!entry.name().empty()) {
        outputResult(dnsRequest->ip, entry.name(), useJson, useXml);
    } else {
        outputResult(dnsRequest->ip, "No hostname found", useJson, useXml);
    }
}

static void asyncReverseDnsLookup(
    const std::string &ip,
    const std::string &dns_server,
    bool useJson,
    bool useXml) {

    useJsonXml(useJson, useXml);

    auto dnsRequest = std::make_shared<DnsRequest>(ip, dns_server);

    try {
        Poco::Net::SocketAddress sa(ip, 0);
        Poco::Net::HostEntry entry = Poco::Net::DNS::hostByAddress(sa.host());
        onDnsResolvedWrapper(dnsRequest, entry, "");
    } catch (const Poco::Exception &) {
        onDnsResolvedWrapper(dnsRequest, Poco::Net::HostEntry(), "Unable to resolve");
    }
}
    static void reverseDnsScannerAsync(
        const std::string &ips,
        const std::string &dns_server,
        bool useJson,
        bool useXml) {

        std::istringstream iss(ips);
        std::string ip;

        while (std::getline(iss, ip)) {
            if (!ip.empty()) {
                asyncReverseDnsLookup(ip, dns_server, useJson, useXml);
            }
        }
    }

private:
    static inline void outputResult(const std::string &ip, const std::string &result, bool useJson, bool useXml) {
        if (useJson) {
            std::cout << "{\"ip\":\"" << ip << "\",\"result\":\"" << result << "\"}" << std::endl;
        } else if (useXml) {
            std::cout << "<dns><ip>" << ip << "</ip><result>" << result << "</result></dns>" << std::endl;
        } else {
            std::cout << ip << " -> " << result << std::endl;
        }
    }
};

#endif
