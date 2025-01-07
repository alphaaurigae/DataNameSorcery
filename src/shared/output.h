#ifndef OUTPUT_H
#define OUTPUT_H

#include "XMLoutput.h"
#include "XercesResourceManagement.h"
#include "xerces_manager.h"

#include <iostream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>


void initializeOutput(bool useXml, bool useJson, bool useDefault) {
    if (useXml) {
        XercesManager xercesManager;
    } else if (useJson) {
// foo
    } else if (useDefault) {
// bar
    }
}
inline void outputResult(const std::string& ip, const std::string& result, bool useJson, bool useXml) {
    if (useJson) {
        Poco::JSON::Object jsonResult;
        jsonResult.set("ip", ip);
        jsonResult.set("result", result);
        Poco::JSON::Stringifier::stringify(jsonResult, std::cout);
        std::cout << std::endl;
    } else if (useXml) {
        printXmlHeader();
        outputXmlToConsole(ip, result);
        printXmlFooter();
    } else {
        std::cout << ip << " -> " << result << std::endl;
    }
}
#endif