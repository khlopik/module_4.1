#include "led.hpp"
#include "config.hpp"

namespace led
{
    void LedHandler::init(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
    {
        redPin_ = redPin;
        greenPin_ = greenPin;
        bluePin_ = bluePin;
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
        pinMode(LED_BUILTIN, OUTPUT);
        turnOffBuiltInLed();
        updateLeds();
    }

    Color LedHandler::getCurrentColor()
    {
        return selectedColor_;
    }

    uint8_t LedHandler::getCurrentValue() const
    {
        switch (selectedColor_)
        {
        case Color::Red:
            return redValue_;
        case Color::Green:
            return greenValue_;
        case Color::Blue:
            return blueValue_;
        default:
            return 0;
            break;
        }
    }

    void LedHandler::setValue(uint8_t value)
    {
        switch (selectedColor_)
        {
        case Color::Red:
            redValue_ = value;
            break;
        case Color::Green:
            greenValue_ = value;
            break;
        case Color::Blue:
            blueValue_ = value;
        default:
            break;
        }
    }

    void LedHandler::switchColor()
    {
        Serial.println("== LED: switchColor() ==");
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
        updateLeds();
    }

    void LedHandler::updateLeds()
    {
        digitalWrite(redPin_, selectedColor_ == Color::Red);
        digitalWrite(greenPin_, selectedColor_ == Color::Green);
        digitalWrite(bluePin_, selectedColor_ == Color::Blue);
    }

    void LedHandler::turnOnBuiltInLed()
    {
        digitalWrite(LED_BUILTIN, builtInLedActiveLevel_);
    }

    void LedHandler::turnOffBuiltInLed()
    {
        digitalWrite(LED_BUILTIN, builtInLedInactiveLevel_);
    }
} // namespace led