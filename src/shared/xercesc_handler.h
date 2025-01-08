#ifndef XERCESC_HANDLER_H
#define XERCESC_HANDLER_H


#include <stdexcept>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>


namespace xerces_utils {

class XMLString {
public:
    explicit XMLString(const char* str) : data(xercesc::XMLString::transcode(str)) {}
    ~XMLString() { xercesc::XMLString::release(&data); }
    const XMLCh* get() const { return data; }

private:
    XMLCh* data;
};

}

namespace xerces_management {

class XercesResource {
public:
    explicit XercesResource(xercesc::DOMImplementation* impl) 
        : doc(nullptr), rootElem(nullptr), serializer(nullptr), 
          output(nullptr), formatTarget(nullptr), impl(impl) {}

    inline void init() {
        doc = impl->createDocument();
        rootElem = doc->createElement(xercesc::XMLString::transcode("Result"));
        doc->appendChild(rootElem);

        serializer = ((xercesc::DOMImplementationLS*)impl)->createLSSerializer();
        output = ((xercesc::DOMImplementationLS*)impl)->createLSOutput();
        formatTarget = new xercesc::StdOutFormatTarget();

        output->setByteStream(formatTarget);
    }

    ~XercesResource() {
        if (formatTarget) delete formatTarget;
        if (output) output->release();
        if (serializer) serializer->release();
        if (doc) doc->release();
    }

    xercesc::DOMDocument* doc;
    xercesc::DOMElement* rootElem;
    xercesc::DOMLSSerializer* serializer;
    xercesc::DOMLSOutput* output;
    xercesc::XMLFormatTarget* formatTarget;

private:
    xercesc::DOMImplementation* impl;
};

}

#endif