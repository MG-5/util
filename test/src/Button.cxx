#include "util/Button.hpp"
#include "GpioMock.hpp"

#include <gtest/gtest.h>

using ::testing::Eq;
using ::testing::Return;
using ::testing::StrictMock;

class ButtonTest : public ::testing::Test
{
public:
    ButtonTest() = default;

    using CallbackFunctionMock = testing::MockFunction<void(util::Button::Action action)>;
    CallbackFunctionMock callbackFunctionMock;

    StrictMock<GpioMock> buttonGpio{};
    util::Button button{buttonGpio,
                        [this](util::Button::Action action) { callbackFunctionMock.Call(action); },
                        500.0_ms};
};

TEST_F(ButtonTest, Idle)
{
    constexpr auto NumberOfUpdates = 10'000;

    // idle (pin at pulled up)
    ON_CALL(buttonGpio, read).WillByDefault(Return(true));
    EXPECT_CALL(buttonGpio, read).Times(NumberOfUpdates);
    EXPECT_CALL(callbackFunctionMock, Call).Times(0);

    for (size_t i = 0; i < NumberOfUpdates; i++)
        button.update(1.0_ms);
}

TEST_F(ButtonTest, ShortPress)
{
    constexpr auto NumberOfUpdates = 60;

    // pin is pulled down, switch is pressed
    ON_CALL(buttonGpio, read).WillByDefault(Return(false));
    EXPECT_CALL(buttonGpio, read).Times(NumberOfUpdates);

    for (size_t i = 0; i < NumberOfUpdates; i++)
        button.update(1.0_ms);

    // release switch after debounce time
    ON_CALL(buttonGpio, read).WillByDefault(Return(true));
    EXPECT_CALL(buttonGpio, read).Times(1);
    EXPECT_CALL(callbackFunctionMock, Call(Eq(util::Button::Action::ShortPress))).Times(1);
    button.update(1.0_ms);
}

TEST_F(ButtonTest, ShortPressTooShort)
{
    constexpr auto NumberOfUpdates = 40;

    // pin is pulled down, switch is pressed
    ON_CALL(buttonGpio, read).WillByDefault(Return(false));
    EXPECT_CALL(buttonGpio, read).Times(NumberOfUpdates);

    for (size_t i = 0; i < NumberOfUpdates; i++)
        button.update(1.0_ms);

    // release switch after debounce time
    ON_CALL(buttonGpio, read).WillByDefault(Return(true));
    EXPECT_CALL(buttonGpio, read).Times(1);
    EXPECT_CALL(callbackFunctionMock, Call).Times(0);
    button.update(1.0_ms);
}

TEST_F(ButtonTest, LongPress)
{
    constexpr auto NumberOfUpdatesFirstStage = 502;

    // pin is pulled down, switch is pressed
    ON_CALL(buttonGpio, read).WillByDefault(Return(false));
    EXPECT_CALL(buttonGpio, read).Times(NumberOfUpdatesFirstStage);
    EXPECT_CALL(callbackFunctionMock, Call(Eq(util::Button::Action::LongPress))).Times(1);

    for (size_t i = 0; i < NumberOfUpdatesFirstStage; i++)
        button.update(1.0_ms);

    constexpr auto NumberOfUpdatesSecondStage = 10'000;
    EXPECT_CALL(buttonGpio, read).Times(NumberOfUpdatesSecondStage);
    EXPECT_CALL(callbackFunctionMock, Call).Times(0);

    for (size_t i = 0; i < NumberOfUpdatesSecondStage; i++)
        button.update(1.0_ms);

    // release switch
    ON_CALL(buttonGpio, read).WillByDefault(Return(true));
    EXPECT_CALL(buttonGpio, read).Times(1);
    EXPECT_CALL(callbackFunctionMock, Call(Eq(util::Button::Action::StopLongPress))).Times(1);
    button.update(1.0_ms);
}