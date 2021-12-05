#include "wrappers/IndependentWatchdog.hpp"
util::IndependentWatchdog::IndependentWatchdog(IWDG_HandleTypeDef &iwdt) : iwdg(iwdt)
{
}
void util::IndependentWatchdog::refresh()
{
    HAL_IWDG_Refresh(&iwdg);
}