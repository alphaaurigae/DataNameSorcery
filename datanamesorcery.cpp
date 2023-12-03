
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <memory>
#include <utility>
#include <algorithm>
#include <limits>

#include <vector>
#include <string_view>
#include <uv.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Exception.h>
#include <Poco/Net/SocketAddress.h>
#include <nlohmann/json.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/TransService.hpp>

namespace fs = std::filesystem;

void outputResult(const std::string &ip, const std::string &result, bool useJson, bool useXml);

class UVLoop
{
public:
    UVLoop() : loop(uv_default_loop()) {}

    UVLoop(const UVLoop &) = delete;
    UVLoop &operator=(const UVLoop &) = delete;

    UVLoop(UVLoop &&other) noexcept : loop(other.loop)
    {
        other.loop = nullptr;
    }

    UVLoop &operator=(UVLoop &&other) noexcept
    {
        if (this != &other)
        {
            if (loop)
            {
                uv_loop_close(loop);
            }

            loop = other.loop;
            other.loop = nullptr;
        }
        return *this;
    }

    ~UVLoop() noexcept
    {
        if (loop)
        {
            uv_loop_close(loop);
        }
    }

    uv_loop_t *get() const
    {
        return loop;
    }

private:
    uv_loop_t *loop;
};
class FileReader
{
public:
    explicit FileReader(const std::string &filename)
    {
        if (!fs::exists(filename))
        {
            throw std::runtime_error("Error: File " + filename + " does not exist");
        }

        if (!fs::is_regular_file(filename))
        {
            throw std::runtime_error("Error: " + filename + " is not a regular file");
        }

        file = std::make_unique<std::ifstream>(filename, std::ios::binary); // Binary mode
        if (!file->is_open())
        {
            throw std::runtime_error("Error: Unable to open " + filename);
        }
    }

    std::string readFileLines() const
    {
        std::string lines;
        std::string line;

        while (std::getline(*file, line))
        {
            if (file->eof())
            {
                break;
            }
            processLine(line, lines);
        }

        return lines;
    }

private:
    void processLine(std::string line, std::string &lines) const
    {
        size_t pos = line.find("//");
        if (pos != std::string::npos)
        {
            line = line.substr(0, pos);
        }

        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch)
                                              { return !std::isspace(ch); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch)
                                { return !std::isspace(ch); })
                       .base(),
                   line.end());

        if (!line.empty())
        {
            try
            {
                Poco::Net::IPAddress ip(line);
                if (ip.family() == Poco::Net::IPAddress::IPv4 ||
                    ip.family() == Poco::Net::IPAddress::IPv6)
                {
                    lines += line + '\n';
                }
                else
                {
                    Poco::Net::SocketAddress address(line, 80);
                    lines += line + '\n';
                }
            }
            catch (const Poco::Exception &)
            {
                lines += "Invalid input: " + line + '\n';
            }
        }
    }

    mutable std::unique_ptr<std::ifstream> file;
};

void outputXmlToConsole(const std::string &ip, const std::string &result);

class XercesManager
{
public:
    XercesManager()
    {
        initializeXerces();
    }

    ~XercesManager()
    {
        terminateXerces();
    }

    static void initializeXerces()
    {
        try
        {
            xercesc::XMLPlatformUtils::Initialize();
        }
        catch (const xercesc::XMLException &e)
        {
            char *message = xercesc::XMLString::transcode(e.getMessage());
            std::cerr << "Xerces-C++ initialization error: " << message << std::endl;
            xercesc::XMLString::release(&message);
        }
    }

    static void terminateXerces()
    {
        xercesc::XMLPlatformUtils::Terminate();
    }
};

void initializeXerces()
{
    XercesManager::initializeXerces();
}

void terminateXerces()
{
    XercesManager::terminateXerces();
}

void printXmlHeader()
{
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::cout << "<ReverseDnsResults>\n";
}

void printXmlFooter()
{
    std::cout << "</ReverseDnsResults>\n";
}

void outputXmlToConsole(const std::string &ip, const std::string &result)
{
    std::cout << "  <Result>\n";
    std::cout << "    <IP>" << ip << "</IP>\n";
    std::cout << "    <ResultText>" << result << "</ResultText>\n";
    std::cout << "  </Result>\n";
}

void outputResult(const std::string &ip, const std::string &result, bool useJson, bool useXml)
{
    if (useJson)
    {
        nlohmann::json jsonResult;
        jsonResult["ip"] = ip;
        jsonResult["result"] = result;
        std::cout << jsonResult.dump() << std::endl;
    }
    else if (useXml)
    {
        outputXmlToConsole(ip, result);
    }
    else
    {
        std::cout << ip << " -> " << result << std::endl;
    }
}

class DnsRequest
{
public:
    std::string ip;
    std::string dns_server;

    DnsRequest(const std::string &ip, const std::string &dns_server)
        : ip(ip), dns_server(dns_server) {}
};

class DnsResolver
{
private:
    static bool useJson;
    static bool useXml;

public:
    static void setUseJson(bool value)
    {
        useJson = value;
    }

    static void setUseXml(bool value)
    {
        useXml = value;
    }

    static bool getUseJson()
    {
        return useJson;
    }

    static bool getUseXml()
    {
        return useXml;
    }

    static void onDnsResolvedWrapper(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
    {
        std::ostream &output = std::cout;
        onDnsResolved(static_cast<uv_getaddrinfo_t *>(req), status, res, output);
        uv_freeaddrinfo(res);
    }

    static void onDnsResolved(uv_getaddrinfo_t *req, int status, struct addrinfo *res, std::ostream &output)
    {
        std::unique_ptr<DnsRequest> dnsRequest(reinterpret_cast<DnsRequest *>(req->data));

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
};

void asyncReverseDnsLookup(
    const std::string &ip,
    const std::string &dns_server,
    uv_loop_t *const loop,
    bool useJson,
    bool useXml)
{
    auto request = std::make_unique<uv_getaddrinfo_t>();
    auto dnsRequest = std::make_unique<DnsRequest>(ip, dns_server);
    request->data = dnsRequest.get();

    struct addrinfo hints;
    std::fill(
        reinterpret_cast<char *>(&hints),
        reinterpret_cast<char *>(&hints) + sizeof(hints),
        0);
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
        // Transfer ownership to libuv
        request.release();
        dnsRequest.release();
    }
}

void reverseDnsScannerAsync(
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
        asyncReverseDnsLookup(ip, dns_server, loop, useJson, useXml);
    }
}

bool DnsResolver::useJson = false;
bool DnsResolver::useXml = false;

enum class FlagType
{
    HOST_DOMAIN,
    DNS,
    DEF,
    JSON,
    XML
};

const std::string_view HOST_DOMAIN_FLAG = "-hst";
const std::string_view DNS_FLAG = "-dns";
const std::string_view DEF_FLAG = "-def";
const std::string_view JSON_FLAG = "-json";
const std::string_view XML_FLAG = "-xml";

void parseCommandLineArguments(int argc, char *argv[], std::string &hostDomainFile, std::string &dnsFile, bool &useJson, bool &useXml)
{
    if (argc != 6 || (argv[1] != HOST_DOMAIN_FLAG && argv[1] != DNS_FLAG) ||
        (argv[3] != HOST_DOMAIN_FLAG && argv[3] != DNS_FLAG) ||
        (argv[5] != DEF_FLAG && argv[5] != JSON_FLAG && argv[5] != XML_FLAG))
    {
        throw std::runtime_error(
            "Usage: " +
            std::string(argv[0]) +
            " " +
            HOST_DOMAIN_FLAG.data() +
            " <host_domain_file> " +
            DNS_FLAG.data() +
            " <dns_file> {" +
            DEF_FLAG.data() +
            " | " +
            JSON_FLAG.data() +
            " | " +
            XML_FLAG.data() +
            "}\n" +
            HOST_DOMAIN_FLAG.data() +
            " <host_domain_file> == a file with ipV4 or ipv6 to reverse-dns scan\n" +
            DNS_FLAG.data() +
            " <dns_file> == a file with dns servers in ipV4, ipv6, or domain format\n" +
            DEF_FLAG.data() +
            " == use default output\n" +
            JSON_FLAG.data() +
            " == use JSON output\n" +
            XML_FLAG.data() +
            " == use XML output");
    }

    FlagType hostDomainFlag = (argv[1] == HOST_DOMAIN_FLAG) ? FlagType::HOST_DOMAIN : FlagType::DNS;
    FlagType dnsFlag = (argv[3] == DNS_FLAG) ? FlagType::DNS : FlagType::HOST_DOMAIN;
    FlagType outputFlag = (argv[5] == DEF_FLAG) ? FlagType::DEF : ((argv[5] == JSON_FLAG) ? FlagType::JSON : FlagType::XML);

    hostDomainFile = argv[(hostDomainFlag == FlagType::HOST_DOMAIN) ? 2 : 4];
    dnsFile = argv[(dnsFlag == FlagType::DNS) ? 4 : 2];

    useJson = (outputFlag == FlagType::JSON);
    useXml = (outputFlag == FlagType::XML);
}

int main(int argc, char *argv[])
{
    try
    {
        std::string hostDomainFile, dnsFile;
        bool useJson = false;
        bool useXml = false;

        parseCommandLineArguments(argc, argv, hostDomainFile, dnsFile, useJson, useXml);

        DnsResolver::setUseJson(useJson);
        DnsResolver::setUseXml(useXml);

        initializeXerces();

        FileReader ipFileReader(hostDomainFile);
        std::string ips = ipFileReader.readFileLines();

        FileReader dnsFileReader(dnsFile);
        std::string dns_server = dnsFileReader.readFileLines();

        if (dns_server.empty())
        {
            throw std::runtime_error("Error: DNS file is empty");
        }

        if (useXml)
        {
            printXmlHeader();
        }

        UVLoop uvLoop;
        reverseDnsScannerAsync(ips, dns_server, uvLoop.get(), useJson, useXml);

        if (useXml)
        {
            printXmlFooter();
        }

        uv_run(uvLoop.get(), UV_RUN_DEFAULT);

        if (useXml)
        {
            printXmlFooter();
        }

        terminateXerces();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Oops!: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
