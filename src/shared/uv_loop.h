#ifndef UVLOOP_H
#define UVLOOP_H

#include <uv.h>

class UVLoop {
public:
    UVLoop();
    UVLoop(UVLoop&& other) noexcept;
    UVLoop& operator=(UVLoop&& other) noexcept;
    ~UVLoop() noexcept;

    uv_loop_t* get() const;

private:
    uv_loop_t* loop;
};

inline UVLoop::UVLoop() : loop(uv_default_loop()) {}

inline UVLoop::UVLoop(UVLoop &&other) noexcept : loop(other.loop)
{
    other.loop = nullptr;
}

inline UVLoop& UVLoop::operator=(UVLoop &&other) noexcept
{
    if (this != &other)
    {
        if (loop)
        {
            uv_loop_close(loop);
        }

        loop = other.loop;
        other.loop = nullptr;
    }
    return *this;
}

inline UVLoop::~UVLoop() noexcept
{
    if (loop)
    {
        uv_loop_close(loop);
    }
}

inline uv_loop_t* UVLoop::get() const
{
    return loop;
}

#endif