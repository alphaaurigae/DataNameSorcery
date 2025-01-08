#ifndef OUTPUT_H
#define OUTPUT_H


#include "xml_output.h"
#include "json_output.h"

#include <iostream>


inline void outputResult(const std::string& ip, const std::string& result, bool useJson, bool useXml) {
    if (useJson) {
        json_output::outputJsonToConsole(ip, result);
    } else if (useXml) {
        xml_output::printXmlHeader();
        xml_output::outputXmlToConsole(ip, result);
        xml_output::printXmlFooter();
    } else {
        std::cout << ip << " -> " << result << std::endl;
    }
}

#endif