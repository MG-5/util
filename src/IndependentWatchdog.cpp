#include "wrappers/IndependentWatchdog.hpp"

#ifdef HAL_IWDG_MODULE_ENABLED
namespace util::wrappers
{
IndependentWatchdog::IndependentWatchdog(IWDG_HandleTypeDef &iwdt) : iwdg(iwdt)
{
}
void IndependentWatchdog::refresh()
{
    HAL_IWDG_Refresh(&iwdg);
}
} // namespace util::wrappers
#endif