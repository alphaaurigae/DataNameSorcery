#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <CLI/CLI.hpp>

inline void parse_arguments(CLI::App& app, int argc, char* argv[]) {
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        app.exit(e);
        std::exit(1);
    }
}

#endif