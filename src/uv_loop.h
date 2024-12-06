#ifndef UVLOOP_H
#define UVLOOP_H

#include <uv.h>

class UVLoop
{
public:
    UVLoop(); // constructor
    UVLoop(const UVLoop &) = delete; // disable copy constructor
    UVLoop &operator=(const UVLoop &) = delete; // disable copy assignment
    UVLoop(UVLoop &&other) noexcept; // move constructor
    UVLoop &operator=(UVLoop &&other) noexcept; // move assignment operator
    ~UVLoop() noexcept; // destructor

    uv_loop_t *get() const; // getter for the loop

private:
    uv_loop_t *loop; // pointer to the uv_loop_t object
};

#endif