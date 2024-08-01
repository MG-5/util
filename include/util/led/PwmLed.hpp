#pragma once

#include "LedBase.hpp"
#include "util/PwmOutput.hpp"

namespace util::led::pwm
{

//--------------------------------------------------------------------------------------------------
template <size_t NumberOfResolutionBits>
class SingleLed : public LedBase
{

public:
    explicit SingleLed(PwmOutput<NumberOfResolutionBits> pwmOutput) : pwmOutput(pwmOutput) {};

    void startPwmTimer()
    {
        pwmOutput.startPwmTimer();
    }

private:
    void update() override
    {
        if (isOn)
            pwmOutput.setPwmToMaximum();
        else
            pwmOutput.setPwmValue(0);
    }

    PwmOutput<NumberOfResolutionBits> pwmOutput;
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
            PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput)
        : ledRedPwmOutput(ledRedPwmOutput), ledGreenPwmOutput(ledGreenPwmOutput) {};

    void startPwmTimer()
    {
        ledRedPwmOutput.startPwmTimer();
        ledGreenPwmOutput.startPwmTimer();
    }

private:
    void update() override
    {
        if (isOn)
        {
            switch (currentColor)
            {
            case DualLedColor::Red:
                ledRedPwmOutput.setPwmToMaximum();
                ledGreenPwmOutput.setPwmValue(0);
                break;

            case DualLedColor::Yellow:
                ledRedPwmOutput.setPwmToMaximum();
                ledGreenPwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1) / 3);
                break;

            case DualLedColor::Orange:
                ledRedPwmOutput.setPwmToMaximum();
                ledGreenPwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1) / 8);
                break;

            case DualLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmToMaximum();
                break;

            default:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                break;
            }
        }
        else
        {
            ledRedPwmOutput.setPwmValue(0);
            ledGreenPwmOutput.setPwmValue(0);
        }
    }

    PwmOutput<NumberOfResolutionBits> ledRedPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput;
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
              PwmOutput<NumberOfResolutionBits> ledBluePwmOutput)
        : ledRedPwmOutput{ledRedPwmOutput}, ledGreenPwmOutput{ledGreenPwmOutput},
          ledBluePwmOutput{ledBluePwmOutput} {};

    void startPwmTimer()
    {
        ledRedPwmOutput.startPwmTimer();
        ledGreenPwmOutput.startPwmTimer();
        ledBluePwmOutput.startPwmTimer();
    }

private:
    void update() override
    {
        if (isOn)
        {
            switch (currentColor)
            {
            case TripleLedColor::Red:
                ledRedPwmOutput.setPwmToMaximum();
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Yellow:
                ledRedPwmOutput.setPwmToMaximum();
                ledGreenPwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1) / 3);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Orange:
                ledRedPwmOutput.setPwmToMaximum();
                ledGreenPwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1) / 8);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmToMaximum();
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Blue:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmToMaximum();
                break;

            case TripleLedColor::Turquoise:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmToMaximum();
                ledBluePwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1) / 2);
                break;

            case TripleLedColor::Purple:
                ledRedPwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1));
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(((1 << NumberOfResolutionBits) - 1) / 2);
                break;

            default:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(0);
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

    PwmOutput<NumberOfResolutionBits> ledRedPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledGreenPwmOutput;
    PwmOutput<NumberOfResolutionBits> ledBluePwmOutput;
};

} // namespace util::led::pwm