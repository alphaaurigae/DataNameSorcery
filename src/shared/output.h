#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "XMLoutput.h"  // For XML output functions
#include "XercesResourceManagement.h"  // For Xerces resources

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>


void outputResult(const std::string& ip, const std::string& result, bool useJson, bool useXml) {
    if (useJson) {
        nlohmann::json jsonResult;
        jsonResult["ip"] = ip;
        jsonResult["result"] = result;
        std::cout << jsonResult.dump() << std::endl;
    } else if (useXml) {
        outputXmlToConsole(ip, result);  // From XmlOutput.h
    } else {
        std::cout << ip << " -> " << result << std::endl;  // Plain text output
    }
}

#endif