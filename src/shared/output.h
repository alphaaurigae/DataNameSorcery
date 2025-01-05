#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include "XMLoutput.h"
#include "XercesResourceManagement.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>

#include <iostream>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>

inline void outputResult(const std::string& ip, const std::string& result, bool useJson, bool useXml) {
    if (useJson) {
        Poco::JSON::Object jsonResult;
        jsonResult.set("ip", ip);
        jsonResult.set("result", result);
        Poco::JSON::Stringifier::stringify(jsonResult, std::cout);
        std::cout << std::endl;
    } else if (useXml) {
        outputXmlToConsole(ip, result);
    } else {
        std::cout << ip << " -> " << result << std::endl;
    }
}

#endif