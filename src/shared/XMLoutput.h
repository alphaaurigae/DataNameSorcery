#ifndef XMLOUTPUT_H
#define XMLOUTPUT_H


#include "XercesResourceManagement.h"

#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>


namespace xml_output {

using xerces_utils::XMLString;
using xerces_management::XercesResource;

inline void initializeXerces() {
    try {
        xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException& e) {
        char* message = xercesc::XMLString::transcode(e.getMessage());
        std::cerr << "Error initializing Xerces: " << message << std::endl;
        xercesc::XMLString::release(&message);
        throw std::runtime_error("Xerces initialization failed.");
    }
}

inline void printXmlHeader() {
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::cout << "<ReverseDnsResults>\n";
}

inline void printXmlFooter() {
    std::cout << "</ReverseDnsResults>\n";
}

inline void outputXmlToConsole(const std::string& ip, const std::string& result) {
    try {
        initializeXerces();

        XMLString ipElement("IP");
        XMLString resultElement("ResultText");
        XMLString ipValue(ip.c_str());
        XMLString resultValue(result.c_str());

        xercesc::DOMImplementation* impl = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode("LS"));
        if (!impl) {
            throw std::runtime_error("Unable to get DOMImplementation for Xerces.");
        }

        XercesResource resources(impl);
        resources.init();

        xercesc::DOMElement* ipElem = resources.doc->createElement(ipElement.get());
        ipElem->appendChild(resources.doc->createTextNode(ipValue.get()));
        resources.rootElem->appendChild(ipElem);

        xercesc::DOMElement* resultElem = resources.doc->createElement(resultElement.get());
        resultElem->appendChild(resources.doc->createTextNode(resultValue.get()));
        resources.rootElem->appendChild(resultElem);

        resources.serializer->write(resources.doc, resources.output);

    } catch (const xercesc::XMLException& e) {
        char* message = xercesc::XMLString::transcode(e.getMessage());
        std::cerr << "XML Exception: " << message << std::endl;
        xercesc::XMLString::release(&message);
    } catch (const xercesc::DOMException& e) {
        char* message = xercesc::XMLString::transcode(e.msg);
        std::cerr << "DOM Exception: " << message << std::endl;
        xercesc::XMLString::release(&message);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

}

#endif