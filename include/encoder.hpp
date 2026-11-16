#pragma once

#include "RotaryEncoder.h"
#include <optional>

namespace encoder
{
    using EventCallback = void (*)(uint8_t);

    class Encoder
    {
    public:
        void init(uint8_t clkPin, uint8_t dtPin);
        void update();
        long getPosition();
        void resetPosition();
        void setCallback(EventCallback cb);

    private:
        std::optional<RotaryEncoder> rotaryEncoder_;
        EventCallback callback_ = nullptr;
        uint8_t currentPosition_ = 0;
        static constexpr uint8_t MAX_POSITION = 255;
    };
} // namespace encoder