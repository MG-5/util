#pragma once

#include <FreeRTOS.h>
#include <limits>

namespace util
{
template <size_t NumberOfResolutionBits>
class PwmOutput
{
public:
    PwmOutput(TIM_HandleTypeDef *const timerHandle, const uint32_t channelNumber)
        : TimerHandle(timerHandle), ChannelNumber(channelNumber)
    {
        configASSERT(TimerHandle != nullptr);
        startPwmTimer();
    };

    void startPwmTimer() const
    {
        HAL_TIM_PWM_Start(TimerHandle, ChannelNumber);
    }

    void setPwmValue(size_t pwmValue) const
    {
        __HAL_TIM_SET_COMPARE(TimerHandle, ChannelNumber, pwmValue);
    }

    size_t getMaximumPwmValue() const
    {
        return (1 << NumberOfResolutionBits) - 1;
    }

    void setPwmToMaximum() const
    {
        __HAL_TIM_SET_COMPARE(TimerHandle, ChannelNumber, getMaximumPwmValue());
    }

private:
    TIM_HandleTypeDef *const TimerHandle;
    const uint32_t ChannelNumber;
};
} // namespace util