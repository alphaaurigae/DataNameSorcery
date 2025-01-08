#ifndef MENU_GENERIC_H
#define MENU_GENERIC_H

#include <string>
#include <CLI/CLI.hpp>

struct AppSettings {
    std::string hostDomainFile;
    std::string dnsFile;
    bool useDefault = false;
    bool useJson = false;
    bool useXml = false;
    bool show_man = false;


    CLI::Option* man_opt = nullptr;
    CLI::Option* def_opt = nullptr; 
    CLI::Option* json_opt = nullptr; 
    CLI::Option* xml_opt = nullptr; 

    void setup_app(CLI::App& app) {
        auto host_opt = app.add_option("--host", hostDomainFile, "HOST file");
        auto target_opt = app.add_option("--target", dnsFile, "TARGET file");
        host_opt->needs(target_opt);
        target_opt->needs(host_opt);

        def_opt = app.add_flag("--def", useDefault, "Enable default output");
        json_opt = app.add_flag("--json", useJson, "Enable JSON output");
        xml_opt = app.add_flag("--xml", useXml, "Enable XML output");

        def_opt->excludes(json_opt);
        def_opt->excludes(xml_opt);
        json_opt->excludes(def_opt);
        json_opt->excludes(xml_opt);
        xml_opt->excludes(def_opt);
        xml_opt->excludes(json_opt);

        man_opt = app.add_flag("--man", show_man, "Display detailed manual");
        
        man_opt->excludes(def_opt);
        man_opt->excludes(json_opt);
        man_opt->excludes(xml_opt);
        man_opt->excludes(host_opt);
        man_opt->excludes(target_opt);
    }
};

#endif