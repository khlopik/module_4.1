#pragma once

#include "Arduino.h"
#include <stdint.h>

namespace ledConfig
{
    constexpr uint8_t pinRed = PA1;
    constexpr uint8_t pinGreen = PA2;
    constexpr uint8_t pinBlue = PA3;
}; // namespace ledCondig

namespace buttonConfig
{
    constexpr uint8_t pin = PA0;
} // namespace buttonConfig

namespace encoderConfig
{
    constexpr uint8_t clkPin = PA4;
    constexpr uint8_t dtPin = PA5;
} // namespace encoderConfig

namespace uartConfig
{
    constexpr uint8_t rxPin = PA10;
    constexpr uint8_t txPin = PA9;
}