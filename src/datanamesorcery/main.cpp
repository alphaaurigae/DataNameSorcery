
#include "command_line_parser.h"
#include "DnsResolver.h"
#include "output.h"
#include "xerces_manager.h"
#include "input.h"

#include <iostream>

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

        XercesManager xercesManager;

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

        DnsResolver::reverseDnsScannerAsync(ips, dns_server, useJson, useXml);

        if (useXml)
        {
            printXmlFooter();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Oops!: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
