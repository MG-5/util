#pragma once
#include "tim.h"

#include <limits>

template <typename TimerResolution>
class PwmOutput
{
public:
    PwmOutput(TIM_HandleTypeDef *timerHandle, uint8_t channelNumber)
        : timerHandle(timerHandle), channelNumber(channelNumber){};

    void setPwmValue(TimerResolution pwmValue)
    {
        __HAL_TIM_SET_COMPARE(timerHandle, channelNumber, pwmValue);
    }

    void setMaximumPwm()
    {
        __HAL_TIM_SET_COMPARE(timerHandle, channelNumber,
                              std::numeric_limits<TimerResolution>::max());
    }

private:
    TIM_HandleTypeDef *timerHandle;
    uint32_t channelNumber;
};