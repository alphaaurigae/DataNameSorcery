#ifndef MENU_GENERIC_H
#define MENU_GENERIC_H

#include <string>
#include <CLI/CLI.hpp>

struct AppSettings {
    std::string hostDomainFile;
    std::string dnsFile;
    bool useDefault = false;
    bool useXml = false;
    bool useJson = false;
    bool show_man = false;

    CLI::Option* man_opt = nullptr;  // Magic to build with man_opt without moving it to the exec source file.

    void setup_app(CLI::App& app) {
        app.add_flag("--def", useDefault, "Enable default output");
        app.add_option("--host", hostDomainFile, "HOST file");
        app.add_option("--target", dnsFile, "TARGET file");
        app.add_flag("--xml", useXml, "Enable XML output");
        app.add_flag("--json", useJson, "Enable JSON output");
        man_opt = app.add_flag("--man", show_man, "Display detailed manual");
    }
};

#endif