#pragma once

#include <stdint.h>
#include <Arduino.h>

namespace led
{
    enum class Color
    {
        Red,
        Green,
        Blue,
    };

    class LedHandler
    {
    public:
        void init(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
        Color getCurrentColor();
        uint8_t getCurrentValue() const;
        void switchColor();
        void setValue(uint8_t value);
        void updateLeds();
        void turnOnBuiltInLed();
        void turnOffBuiltInLed();

    private:
        uint8_t redPin_;
        uint8_t greenPin_;
        uint8_t bluePin_;
        Color selectedColor_ = Color::Red;
        uint8_t redValue_ = 127;
        uint8_t greenValue_ = 127;
        uint8_t blueValue_ = 127;
        int builtInLedActiveLevel_ = LOW;
        int builtInLedInactiveLevel_ = HIGH;
    };
} // namespace led