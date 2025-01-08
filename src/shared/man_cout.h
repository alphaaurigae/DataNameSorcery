#ifndef COUT_MAN_H
#define COUT_MAN_H

#include "man_main.h"
#include <iostream>


void exec_man();

inline void cout_man(bool show_man) {
    if (show_man) {
        main_man();
        exec_man();
        std::exit(0);
    }
}

#endif