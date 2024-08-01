#pragma once

#include "GammaCorrection.hpp"
#include "LedBase.hpp"
#include "util/PwmOutput.hpp"

namespace util::led::pwm
{

//--------------------------------------------------------------------------------------------------
template <size_t NumberOfResolutionBits>
class SingleLed : public LedBase
{

public:
    explicit SingleLed(PwmOutput<NumberOfResolutionBits> pwmOutput,
                       const GammaCorrection<NumberOfResolutionBits> &gammaCorrection)
        : pwmOutput(pwmOutput), gammaCorrection(gammaCorrection) {};

    void startPwmTimer()
    {
        pwmOutput.startPwmTimer();
    }

    void setBrightness(uint8_t newBrightness)
    {
        newBrightness > 100 ? brightness = 100 : brightness = newBrightness;
    }

private:
    void update() override
    {
        if (isOn)
            pwmOutput.setPwmValue(
                applyBrightnessAndGammaCorrection(pwmOutput.getMaximumPwmValue()));
        else
            pwmOutput.setPwmValue(0);
    }

    size_t applyBrightnessAndGammaCorrection(size_t pwmValue)
    {
        const auto BrightnessCorrectedPwmValue = (pwmValue * brightness) / 100;

        return gammaCorrection.GammaCorrectionLUT[BrightnessCorrectedPwmValue];
    }

    PwmOutput<NumberOfResolutionBits> pwmOutput;
    const GammaCorrection<NumberOfResolutionBits> &gammaCorrection;

    uint8_t brightness = 100;
};

//--------------------------------------------------------------------------------------------------
enum class DualLedColor
{
    Red,
    Green,
    Yellow,
    Orange
};

//--------------------------------------------------------------------------------------------------
template <size_t NumberOfResolutionBits>
class DualLed : public MultiColorLedBase<DualLedColor>
{
public:
    DualLed(PwmOutput<NumberOfResolutionBits> ledRedPwmOutput,
            PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput,
            const GammaCorrection<NumberOfResolutionBits> &gammaCorrection)
        : ledRedPwmOutput(ledRedPwmOutput), ledGreenPwmOutput(ledGreenPwmOutput),
          gammaCorrection(gammaCorrection) {};

    void startPwmTimer()
    {
        ledRedPwmOutput.startPwmTimer();
        ledGreenPwmOutput.startPwmTimer();
    }

    void setBrightness(uint8_t newBrightness)
    {
        newBrightness > 100 ? brightness = 100 : brightness = newBrightness;
    }

private:
    void update() override
    {
        if (isOn)
        {
            const auto MaxPwmValue = ledGreenPwmOutput.getMaximumPwmValue();

            switch (currentColor)
            {
            case DualLedColor::Red:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(0);
                break;

            case DualLedColor::Yellow:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(
                    applyBrightnessAndGammaCorrection(MaxPwmValue * 100 / 140));
                break;

            case DualLedColor::Orange:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue / 2));
                break;

            case DualLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                break;
            }
        }
        else
        {
            ledRedPwmOutput.setPwmValue(0);
            ledGreenPwmOutput.setPwmValue(0);
        }
    }

    size_t applyBrightnessAndGammaCorrection(size_t pwmValue)
    {
        const auto BrightnessCorrectedPwmValue = (pwmValue * brightness) / 100;

        return gammaCorrection.GammaCorrectionLUT[BrightnessCorrectedPwmValue];
    }

    PwmOutput<NumberOfResolutionBits> ledRedPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput;
    const GammaCorrection<NumberOfResolutionBits> &gammaCorrection;

    uint8_t brightness = 100;
};

//--------------------------------------------------------------------------------------------------
enum class TripleLedColor
{
    Red,
    Green,
    Blue,
    Yellow,
    Orange,
    Purple,
    Turquoise
};

//--------------------------------------------------------------------------------------------------
template <size_t NumberOfResolutionBits>
class TripleLed : public MultiColorLedBase<TripleLedColor>
{
public:
    TripleLed(PwmOutput<NumberOfResolutionBits> ledRedPwmOutput,
              PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput,
              PwmOutput<NumberOfResolutionBits> ledBluePwmOutput,
              const GammaCorrection<NumberOfResolutionBits> &gammaCorrection)
        : ledRedPwmOutput(ledRedPwmOutput), ledGreenPwmOutput(ledGreenPwmOutput),
          ledBluePwmOutput(ledBluePwmOutput), gammaCorrection(gammaCorrection) {};

    void startPwmTimer()
    {
        ledRedPwmOutput.startPwmTimer();
        ledGreenPwmOutput.startPwmTimer();
        ledBluePwmOutput.startPwmTimer();
    }

    void setBrightness(uint8_t newBrightness)
    {
        newBrightness > 100 ? brightness = 100 : brightness = newBrightness;
    }

private:
    void update() override
    {
        if (isOn)
        {
            const auto MaxPwmValue = ledGreenPwmOutput.getMaximumPwmValue();

            switch (currentColor)
            {
            case TripleLedColor::Red:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Yellow:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(
                    applyBrightnessAndGammaCorrection(MaxPwmValue * 100 / 140));
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Orange:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue / 2));
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Blue:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                break;

            case TripleLedColor::Turquoise:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledBluePwmOutput.setPwmValue(
                    applyBrightnessAndGammaCorrection(MaxPwmValue * 100 / 140));
                break;

            case TripleLedColor::Purple:
                ledRedPwmOutput.setPwmValue(applyBrightnessAndGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(
                    applyBrightnessAndGammaCorrection(MaxPwmValue * 100 / 140));
                break;
            }
        }
        else
        {
            ledRedPwmOutput.setPwmValue(0);
            ledGreenPwmOutput.setPwmValue(0);
            ledBluePwmOutput.setPwmValue(0);
        }
    }

    size_t applyBrightnessAndGammaCorrection(size_t pwmValue)
    {
        const auto BrightnessCorrectedPwmValue = (pwmValue * brightness) / 100;

        return gammaCorrection.GammaCorrectionLUT[BrightnessCorrectedPwmValue];
    }

    PwmOutput<NumberOfResolutionBits> ledRedPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledBluePwmOutput;
    const GammaCorrection<NumberOfResolutionBits> &gammaCorrection;

    uint8_t brightness = 100;
};
} // namespace util::led::pwm