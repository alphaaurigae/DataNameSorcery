#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <string>
#include <stdexcept>

enum class FlagType
{
    HOST_DOMAIN,
    DNS,
    DEF,
    JSON,
    XML
};

extern const std::string_view HOST_DOMAIN_FLAG;
extern const std::string_view DNS_FLAG;
extern const std::string_view DEF_FLAG;
extern const std::string_view JSON_FLAG;
extern const std::string_view XML_FLAG;

void parseCommandLineArguments(int argc, char *argv[], std::string &hostDomainFile, std::string &dnsFile, bool &useJson, bool &useXml);

#endif