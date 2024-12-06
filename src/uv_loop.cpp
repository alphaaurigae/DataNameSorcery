#include "uv_loop.h"

UVLoop::UVLoop() : loop(uv_default_loop()) {}

UVLoop::UVLoop(UVLoop &&other) noexcept : loop(other.loop)
{
    other.loop = nullptr;
}

UVLoop &UVLoop::operator=(UVLoop &&other) noexcept
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

UVLoop::~UVLoop() noexcept
{
    if (loop)
    {
        uv_loop_close(loop);
    }
}

uv_loop_t *UVLoop::get() const
{
    return loop;
}