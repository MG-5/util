#pragma once
#include "tim.h"

#include <limits>

template <typename TimerResolution>
class PwmOutput
{
public:
    constexpr PwmOutput(TIM_HandleTypeDef *const timerHandle, const uint8_t channelNumber)
        : TimerHandle(timerHandle), ChannelNumber(channelNumber){};

    void startPwmTimer() const
    {
        HAL_TIM_PWM_Start(TimerHandle, ChannelNumber);
    }

    void setPwmValue(TimerResolution pwmValue) const
    {
        __HAL_TIM_SET_COMPARE(TimerHandle, ChannelNumber, pwmValue);
    }

    void setMaximumPwm() const
    {
        __HAL_TIM_SET_COMPARE(TimerHandle, ChannelNumber,
                              std::numeric_limits<TimerResolution>::max());
    }

private:
    TIM_HandleTypeDef *const TimerHandle;
    const uint32_t ChannelNumber;
};

using PwmOutput8Bit = PwmOutput<uint8_t>;
using PwmOutput16Bit = PwmOutput<uint16_t>;