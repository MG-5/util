#pragma once

#include "GammaCorrection.hpp"
#include "LedBase.hpp"
#include "util/PwmOutput.hpp"

namespace util::led::pwm
{

//--------------------------------------------------------------------------------------------------
template <size_t NumberOfResolutionBits, class GammaCorrection_t>
class SingleLed : public LedBase
{

public:
    explicit SingleLed(PwmOutput<NumberOfResolutionBits> pwmOutput,
                       const GammaCorrection_t &gammaCorrection)
        : pwmOutput(pwmOutput), gammaCorrection(gammaCorrection) {};

    void fadeLightLevelTo(size_t newLightLevel)
    {
        initialLightLevel = isOn ? currentLightLevel : 0;
        lightLevelDifference = newLightLevel - initialLightLevel;
        currentFadeProgress = 0.0_ms;
        fadingState = true;
        isOn = true;
    }

    void setLightLevel(size_t newLevel)
    {
        isOn = newLevel > 0;
        currentLightLevel =
            (newLevel > pwmOutput.getMaximumPwmValue()) ? pwmOutput.getMaximumPwmValue() : newLevel;
    }

    size_t getLightLevel() const
    {
        return currentLightLevel;
    }

    bool isFading() const
    {
        return fadingState;
    }

private:
    size_t currentLightLevel = 0; /// has the same amount of steps as PWM
    size_t initialLightLevel = 0;
    int16_t lightLevelDifference = 0;

    bool fadingState = false;

    static constexpr units::si::Time DelayPerStep = 1.0_ms;
    static constexpr units::si::Time FadeDuration = 300.0_ms;

    units::si::Time currentFadeProgress = 0.0_ms;

    void update() override
    {
        if (fadingState)
        {
            currentFadeProgress += DelayPerStep;
            currentLightLevel =
                initialLightLevel +
                ((currentFadeProgress / FadeDuration) * lightLevelDifference).getMagnitude();

            if (currentLightLevel > pwmOutput.getMaximumPwmValue())
                currentLightLevel = pwmOutput.getMaximumPwmValue();

            if (currentFadeProgress >= FadeDuration)
            {
                fadingState = false;
            }
        }

        pwmOutput.setPwmValue(isOn ? applyGammaCorrection(currentLightLevel) : 0);
    }

    size_t applyGammaCorrection(size_t pwmValue)
    {
        return gammaCorrection.LookUpTable[pwmValue];
    }

    PwmOutput<NumberOfResolutionBits> pwmOutput;
    const GammaCorrection_t &gammaCorrection;
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
template <size_t NumberOfResolutionBits, class GammaCorrection_t>
class DualLed : public MultiColorLedBase<DualLedColor>
{
public:
    DualLed(PwmOutput<NumberOfResolutionBits> ledRedPwmOutput,
            PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput,
            const GammaCorrection_t &gammaCorrection)
        : ledRedPwmOutput(ledRedPwmOutput), ledGreenPwmOutput(ledGreenPwmOutput),
          gammaCorrection(gammaCorrection) {};

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
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(0);
                break;

            case DualLedColor::Yellow:
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue * 100 / 140));
                break;

            case DualLedColor::Orange:
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue / 2));
                break;

            case DualLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                break;
            }
        }
        else
        {
            ledRedPwmOutput.setPwmValue(0);
            ledGreenPwmOutput.setPwmValue(0);
        }
    }

    size_t applyGammaCorrection(size_t pwmValue)
    {
        const auto BrightnessCorrectedPwmValue = (pwmValue * brightness) / 100;

        return gammaCorrection.LookUpTable[BrightnessCorrectedPwmValue];
    }

    PwmOutput<NumberOfResolutionBits> ledRedPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput;
    const GammaCorrection_t &gammaCorrection;

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
template <size_t NumberOfResolutionBits, class GammaCorrection_t>
class TripleLed : public MultiColorLedBase<TripleLedColor>
{
public:
    TripleLed(PwmOutput<NumberOfResolutionBits> ledRedPwmOutput,
              PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput,
              PwmOutput<NumberOfResolutionBits> ledBluePwmOutput,
              const GammaCorrection<NumberOfResolutionBits> &gammaCorrection)
        : ledRedPwmOutput(ledRedPwmOutput), ledGreenPwmOutput(ledGreenPwmOutput),
          ledBluePwmOutput(ledBluePwmOutput), gammaCorrection(gammaCorrection) {};

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
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Yellow:
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue * 100 / 140));
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Orange:
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue / 2));
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Blue:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                break;

            case TripleLedColor::Turquoise:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledBluePwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue * 100 / 140));
                break;

            case TripleLedColor::Purple:
                ledRedPwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue));
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(applyGammaCorrection(MaxPwmValue * 100 / 140));
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

    size_t applyGammaCorrection(size_t pwmValue)
    {
        const auto BrightnessCorrectedPwmValue = (pwmValue * brightness) / 100;

        return gammaCorrection.LookUpTable[BrightnessCorrectedPwmValue];
    }

    PwmOutput<NumberOfResolutionBits> ledRedPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledBluePwmOutput;
    const GammaCorrection_t &gammaCorrection;

    uint8_t brightness = 100;
};
} // namespace util::led::pwm