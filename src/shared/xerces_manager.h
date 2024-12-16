#ifndef XERCES_MANAGER_H
#define XERCES_MANAGER_H

#include <xercesc/util/XMLString.hpp>
#include <iostream>

class XercesManager {
public:
    XercesManager();
    ~XercesManager() noexcept;

private:
    void initializeXerces();
    void terminateXerces() noexcept;

    class XercesStringGuard {
    public:
        explicit XercesStringGuard(char* str);
        ~XercesStringGuard();
        char* get() const;

    private:
        char* data;
    };
};

inline XercesManager::XercesManager() {
    initializeXerces();
}

inline XercesManager::~XercesManager() noexcept {
    terminateXerces();
}

inline void XercesManager::initializeXerces() {
    try {
        xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException& e) {
        XercesStringGuard message(xercesc::XMLString::transcode(e.getMessage()));
        std::cerr << "Xerces-C++ initialization error: " << message.get() << std::endl;
        throw;
    }
}

inline void XercesManager::terminateXerces() noexcept {
    try {
        xercesc::XMLPlatformUtils::Terminate();
    } catch (...) {
        std::cerr << "Unexpected exception during Xerces termination." << std::endl;
    }
}

inline XercesManager::XercesStringGuard::XercesStringGuard(char* str) : data(str) {}

inline XercesManager::XercesStringGuard::~XercesStringGuard() {
    xercesc::XMLString::release(&data);
}

inline char* XercesManager::XercesStringGuard::get() const {
    return data;
}

#endif