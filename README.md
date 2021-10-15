# util namespace

## Gpio class
*ToDo*

## Led class

#### binary led
A LED can be controlled by simply turning it on or off, without dimming.
This can be represented by `util::binary_led::SingleLed` with `utils::Gpio` as parameter:

```cpp
#include "util/Led.hpp"

constexpr util::Gpio TestLedGpio(TEST_LED_GPIO_Port, TEST_LED_Pin);
util::binary_led::SingleLed testLed(TestLedGpio);
```

#### pwm led

For a multicolor LED it make more sense to controll each LED by PWM to get a various color mixings.
For instance a RGB LED can be represented by `util::pwm_led::TripleLed` with three `utils::PwmOutput8Bit` (for 8-Bit PWM timer) as parameter:

```cpp
#include "util/Led.hpp"

constexpr auto TimerHandle = &htim1;

constexpr PwmOutput8Bit RedChannel(TimerHandle, TIM_CHANNEL_1);
constexpr PwmOutput8Bit GreenChannel(TimerHandle, TIM_CHANNEL_2);
constexpr PwmOutput8Bit BlueChannel(TimerHandle, TIM_CHANNEL_3);

util::pwm_led::TripleLed rgbLed(RedChannel, GreenChannel, BlueChannel);

```

### How to use
You can apply modes like blinking, flashing, on/off state to your object and for multicolor LED you can change its color mix of course.
To make sure that blinking/flashing works properly, the `updateState()` function for each LED object must be called periodically.

```cpp
constexpr auto TaskFrequency = 20.0_Hz;

extern "C" void taskFunction(void *)
{
    // turn on LED and let them blinking with 500ms on/off times
    testLed.setBlinking(1.0_Hz);

    // set LED color to Orange and turn that on
    rgbLed.setColor(util::pwm_led::TripleLedColor::Orange);
    rgbLed.turnOn();

    auto lastWakeTime = xTaskGetTickCount();
    while(true)
    {
        testLed.updateState(lastWakeTime);
        rgbLed.updateState(lastWakeTime);

        vTaskDelayUntil(&lastWakeTime, toOsTicks(TaskFrequency));
    }
}

```

## PwmOutput class
*ToDo*

## Button class
*ToDo*

# unit namespace
*ToDo*