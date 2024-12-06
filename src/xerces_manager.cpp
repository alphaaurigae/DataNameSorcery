#include "xerces_manager.h"
#include <iostream>
#include <xercesc/util/XMLString.hpp>


XercesManager::XercesManager() {
    initializeXerces();
}

XercesManager::~XercesManager() noexcept {
    terminateXerces();
}

void XercesManager::initializeXerces() {
    try {
        xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException& e) {
        XercesStringGuard message(xercesc::XMLString::transcode(e.getMessage()));
        std::cerr << "Xerces-C++ initialization error: " << message.get() << std::endl;
        throw;
    }
}

void XercesManager::terminateXerces() noexcept {
    try {
        xercesc::XMLPlatformUtils::Terminate();
    } catch (...) {
        std::cerr << "Unexpected exception during Xerces termination." << std::endl;
    }
}

XercesManager::XercesStringGuard::XercesStringGuard(char* str) : data(str) {}
XercesManager::XercesStringGuard::~XercesStringGuard() {
    xercesc::XMLString::release(&data);
}
char* XercesManager::XercesStringGuard::get() const {
    return data;
}