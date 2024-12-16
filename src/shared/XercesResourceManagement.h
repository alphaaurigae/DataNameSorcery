#ifndef XERCESRESOURCEMANAGEMENT_H
#define XERCESRESOURCEMANAGEMENT_H


#include <stdexcept>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
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

#endif