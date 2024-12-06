#include "output.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <xercesc/dom/DOM.hpp> 
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

using namespace xercesc;

class XMLStringRAII {
public:
    explicit XMLStringRAII(const char* str) : data(XMLString::transcode(str)) {}
    ~XMLStringRAII() { XMLString::release(&data); }
    const XMLCh* get() const { return data; }

private:
    XMLCh* data;
};

class XercesResourceRAII {
public:
    explicit XercesResourceRAII(DOMImplementation* impl) 
        : impl(impl), doc(nullptr), rootElem(nullptr), serializer(nullptr), output(nullptr), formatTarget(nullptr) {}

    void init() {
        doc = impl->createDocument();
        rootElem = doc->createElement(XMLString::transcode("Result"));
        doc->appendChild(rootElem);
        
        serializer = ((DOMImplementationLS*)impl)->createLSSerializer();
        output = ((DOMImplementationLS*)impl)->createLSOutput();
        formatTarget = new StdOutFormatTarget();
        
        output->setByteStream(formatTarget);
    }

    ~XercesResourceRAII() {
        if (formatTarget) delete formatTarget;
        if (output) output->release();
        if (serializer) serializer->release();
        if (doc) doc->release();
    }

    DOMDocument* doc;
    DOMElement* rootElem;
    DOMLSSerializer* serializer;
    DOMLSOutput* output;
    XMLFormatTarget* formatTarget;

private:
    DOMImplementation* impl;
};

void outputResult(const std::string& ip, const std::string& result, bool useJson, bool useXml) {
    if (useJson) {
        nlohmann::json jsonResult;
        jsonResult["ip"] = ip;
        jsonResult["result"] = result;
        std::cout << jsonResult.dump() << std::endl;
    } else if (useXml) {
        outputXmlToConsole(ip, result);
    } else {
        std::cout << ip << " -> " << result << std::endl;
    }
}

void printXmlHeader() {
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::cout << "<ReverseDnsResults>\n";
}

void printXmlFooter() {
    std::cout << "</ReverseDnsResults>\n";
}

void outputXmlToConsole(const std::string& ip, const std::string& result) {
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