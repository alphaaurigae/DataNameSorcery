#ifndef UVLOOP_H
#define UVLOOP_H

#include <uv.h>

class UVLoop
{
public:
    UVLoop();
    UVLoop(const UVLoop &) = delete;
    UVLoop &operator=(const UVLoop &) = delete;
    UVLoop(UVLoop &&other) noexcept;
    UVLoop &operator=(UVLoop &&other) noexcept;
    ~UVLoop() noexcept;

    uv_loop_t *get() const;

private:
    uv_loop_t *loop;
};

#endif