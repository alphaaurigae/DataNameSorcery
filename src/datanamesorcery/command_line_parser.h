#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H


#include <stdexcept>
#include <string>
#include <string_view>


enum class FlagType {
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

inline void parseCommandLineArguments(int argc, char *argv[], std::string &hostDomainFile, std::string &dnsFile, bool &useJson, bool &useXml)
{
    if (argc != 6 || 
        (std::string_view(argv[1]) != HOST_DOMAIN_FLAG && std::string_view(argv[1]) != DNS_FLAG) ||
        (std::string_view(argv[3]) != HOST_DOMAIN_FLAG && std::string_view(argv[3]) != DNS_FLAG) ||
        (std::string_view(argv[5]) != DEF_FLAG && std::string_view(argv[5]) != JSON_FLAG && std::string_view(argv[5]) != XML_FLAG))
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
            XML_FLAG.data() + "}\n" +
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

    FlagType hostDomainFlag = (std::string_view(argv[1]) == HOST_DOMAIN_FLAG) ? FlagType::HOST_DOMAIN : FlagType::DNS;
    FlagType dnsFlag = (std::string_view(argv[3]) == DNS_FLAG) ? FlagType::DNS : FlagType::HOST_DOMAIN;
    FlagType outputFlag = (std::string_view(argv[5]) == DEF_FLAG) ? FlagType::DEF : ((std::string_view(argv[5]) == JSON_FLAG) ? FlagType::JSON : FlagType::XML);

    hostDomainFile = argv[(hostDomainFlag == FlagType::HOST_DOMAIN) ? 2 : 4];
    dnsFile = argv[(dnsFlag == FlagType::DNS) ? 4 : 2];

    useJson = (outputFlag == FlagType::JSON);
    useXml = (outputFlag == FlagType::XML);
}

#endif