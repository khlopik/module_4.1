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
        void switchColor();
        void turnOnBuiltInLed();
        void turnOffBuiltInLed();

    private:
        Color selectedColor_ = Color::Red;
        int builtInLedActiveLevel_ = LOW;
        int builtInLedInactiveLevel_ = HIGH;
    };
} // namespace led