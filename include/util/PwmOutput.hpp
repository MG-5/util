#pragma once
#include <core/BuildConfiguration.hpp>

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
        SafeAssert(TimerHandle != nullptr);
    };

    void startPwmTimer() const
    {
        HAL_TIM_PWM_Start(TimerHandle, ChannelNumber);
    }

    void setPwmValue(size_t pwmValue) const
    {
        if constexpr (core::BuildConfiguration::IsEmbeddedBuild)
            __HAL_TIM_SET_COMPARE(TimerHandle, ChannelNumber, pwmValue);
    }

    void setMaximumPwm() const
    {
        if constexpr (core::BuildConfiguration::IsEmbeddedBuild)
        {
            __HAL_TIM_SET_COMPARE(TimerHandle, ChannelNumber, (1 << NumberOfResolutionBits) - 1);
        }
    }

private:
    TIM_HandleTypeDef *const TimerHandle;
    const uint32_t ChannelNumber;
};
} // namespace util