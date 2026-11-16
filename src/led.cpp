#include "led.hpp"
#include "config.hpp"

namespace led
{
    void LedHandler::init(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
    {
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
        pinMode(LED_BUILTIN, OUTPUT);
    }

    Color LedHandler::getCurrentColor()
    {
        return selectedColor_;
    }

    void LedHandler::switchColor()
    {
        switch (selectedColor_)
        {
        case Color::Red:
            selectedColor_ = Color::Green;
            break;
        case Color::Green:
            selectedColor_ = Color::Blue;
            break;
        case Color::Blue:
            selectedColor_ = Color::Red;
            break;
        }
    }

    void LedHandler::turnOnBuiltInLed()
    {
        digitalWrite(LED_BUILTIN, buildInLedActiveLevel_);
    }

    void LedHandler::turnOffBuiltInLed()
    {
        digitalWrite(LED_BUILTIN, buildInLedInactiveLevel_);
    }
} // namespace led