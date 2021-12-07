#pragma once
#include <chip.h>

#ifdef HAL_IWDG_MODULE_ENABLED
namespace util
{
class IndependentWatchdog
{
public:
    explicit IndependentWatchdog(IWDG_HandleTypeDef &iwdt);
    void refresh();

private:
    IWDG_HandleTypeDef &iwdg;
};
} // namespace util
#endif