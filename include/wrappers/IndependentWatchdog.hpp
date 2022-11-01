#pragma once

#include <hal_header.h>

#ifdef HAL_IWDG_MODULE_ENABLED
namespace util::wrappers
{
class IndependentWatchdog
{
public:
    explicit IndependentWatchdog(IWDG_HandleTypeDef &iwdt);
    void refresh();

private:
    IWDG_HandleTypeDef &iwdg;
};
} // namespace util::wrappers
#endif