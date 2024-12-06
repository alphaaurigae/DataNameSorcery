#ifndef XERCES_MANAGER_H
#define XERCES_MANAGER_H

#include <xercesc/util/PlatformUtils.hpp>

class XercesManager {
public:
    XercesManager();
    ~XercesManager() noexcept;

private:
    void initializeXerces();
    void terminateXerces() noexcept;

    struct XercesStringGuard {
        explicit XercesStringGuard(char* str);
        ~XercesStringGuard();
        char* get() const;

    private:
        char* data;
    };
};

#endif