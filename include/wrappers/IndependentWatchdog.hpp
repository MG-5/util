#pragma once
#include <chip.h>

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