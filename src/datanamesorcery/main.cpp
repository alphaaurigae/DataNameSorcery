#include "menu_generic.h"
#include "DnsResolver.h"
#include "output.h"
#include "xerces_manager.h"
#include "input.h"
#include "argument_parser.h"
#include "main_man.h"
#include "cout_man.h"

#include <CLI/CLI.hpp>

#include <iostream>
#include <string>
#include <stdexcept>

void exec_man() {
    std::cout << "Usage Examples:\n\n";
    std::cout << "- Reverse DNS Scan (JSON output):\n";
    std::cout << "  bin/scanner --host input_sample/hostfile --target input_sample/dnsfile.txt --def\n\n";
    std::cout << "- Reverse DNS Scan (JSON output):\n";
    std::cout << "  bin/scanner --host input_sample/hostfile --target input_sample/dnsfile.txt --json\n\n";
    std::cout << "- Reverse DNS Scan (XML output):\n";
    std::cout << "  bin/scanner --host input_sample/hostfile.txt --target input_sample/dnsfile.txt --xml\n\n";
}


void process_arguments(CLI::App& app, int argc, char* argv[]) {
    AppSettings settings;
    settings.setup_app(app);

    parse_arguments(app, argc, argv);

    cout_man(settings.show_man);

    std::string hostDomainFile, dnsFile;

    DnsResolver::setUseJson(settings.useJson);
    DnsResolver::setUseXml(settings.useXml);

    FileReader ipFileReader(settings.hostDomainFile);
    std::string ips = ipFileReader.readFileLines();

    FileReader dnsFileReader(settings.dnsFile);
    std::string dns_server = dnsFileReader.readFileLines();

    if (dns_server.empty()) {
        throw std::runtime_error("Error: DNS file is empty");
    }

    initializeOutput(settings.useXml, settings.useJson, settings.useDefault);
    DnsResolver::reverseDnsScannerAsync(ips, dns_server, settings.useJson, settings.useXml);
}



int main(int argc, char* argv[]) {
    CLI::App app{"blah"};
    process_arguments(app, argc, argv);
    return 0;
}
