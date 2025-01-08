#include "menu_generic.h"
#include "dns_resolver.h"
#include "output_handler.h"
#include "xercesc_init.h"
#include "input_handler.h"
#include "argument_parser.h"
#include "man_main.h"
#include "man_cout.h"

#include <CLI/CLI.hpp>

#include <iostream>
#include <string>
#include <stdexcept>


void exec_man() {
    std::cout << "bin/datanamesorcery [host file] [target file] [output format]\n\n";
    std::cout << "Input:\n";
    std::cout << "--host [file \\n] \n";
    std::cout << "--target [file \\n] \n\n";
    std::cout << "Output:\n";
    std::cout << "--def ... default console\n";
    std::cout << "--json ... json format\n";
    std::cout << "--xml ... xml format\n\n";
    std::cout << "bin/datanamesorcery --host input_sample/host --target input_sample/target --def\n";
    std::cout << "bin/datanamesorcery --host input_sample/host --target input_sample/target --json\n";
    std::cout << "bin/datanamesorcery --host input_sample/host --target input_sample/target --xml\n\n";
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

    DnsResolver::reverseDnsScannerAsync(ips, dns_server, settings.useJson, settings.useXml);
}


int main(int argc, char* argv[]) {
    CLI::App app{"datanamesorcery reverse dns scanner"};
    process_arguments(app, argc, argv);
    return 0;
}
