#pragma once
#include <FreeRTOS.h>
#include <hal_header.h>

namespace util::wrappers
{

class HAL
{
public:
    HAL() = default;
    virtual ~HAL() = default;

    HAL(const HAL &) = delete;
    HAL(HAL &&) = delete;
    HAL &operator=(const HAL &) = delete;
    HAL &operator=(HAL &&) = delete;

    virtual uint32_t GetTick() const
    {
        return HAL_GetTick();
    }
};

} // namespace wrapper