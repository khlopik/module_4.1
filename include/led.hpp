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
        void updateLeds();
        void turnOnBuiltInLed();
        void turnOffBuiltInLed();

    private:
        uint8_t redPin_;
        uint8_t greenPin_;
        uint8_t bluePin_;
        Color selectedColor_ = Color::Red;
        int builtInLedActiveLevel_ = LOW;
        int builtInLedInactiveLevel_ = HIGH;
    };
} // namespace led