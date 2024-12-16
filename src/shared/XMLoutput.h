#ifndef XMLOUTPUT_H
#define XMLOUTPUT_H

#include "XercesResourceManagement.h"

#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>



inline void printXmlHeader() {
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::cout << "<ReverseDnsResults>\n";
}

inline void printXmlFooter() {
    std::cout << "</ReverseDnsResults>\n";
}

inline void outputXmlToConsole(const std::string& ip, const std::string& result) {
    try {
        XMLStringRAII ipElement("IP");
        XMLStringRAII resultElement("ResultText");
        XMLStringRAII ipValue(ip.c_str());
        XMLStringRAII resultValue(result.c_str());

        DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("LS"));
        if (!impl) {
            throw std::runtime_error("Unable to get DOMImplementation for Xerces.");
        }

        XercesResourceRAII resources(impl);
        resources.init();

        DOMElement* ipElem = resources.doc->createElement(ipElement.get());
        ipElem->appendChild(resources.doc->createTextNode(ipValue.get()));
        resources.rootElem->appendChild(ipElem);

        DOMElement* resultElem = resources.doc->createElement(resultElement.get());
        resultElem->appendChild(resources.doc->createTextNode(resultValue.get()));
        resources.rootElem->appendChild(resultElem);

        resources.serializer->write(resources.doc, resources.output);

    } catch (const XMLException& e) {
        char* message = XMLString::transcode(e.getMessage());
        std::cerr << "XML Exception: " << message << std::endl;
        XMLString::release(&message);
    } catch (const DOMException& e) {
        char* message = XMLString::transcode(e.msg);
        std::cerr << "DOM Exception: " << message << std::endl;
        XMLString::release(&message);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

#endif