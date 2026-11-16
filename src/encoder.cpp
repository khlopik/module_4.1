#include "encoder.hpp"
#include "RotaryEncoder.h"

namespace encoder
{
    void Encoder::init(uint8_t clkPin, uint8_t dtPin)
    {
        rotaryEncoder_.emplace(clkPin, dtPin, RotaryEncoder::LatchMode::FOUR3);
    }
    void Encoder::update()
    {
        if (!rotaryEncoder_)
        {
            return;
        }

        rotaryEncoder_->tick();

        if (rotaryEncoder_->getDirection() == RotaryEncoder::Direction::NOROTATION)
        {
            return;
        }

        uint8_t position;
        const long newPosition = rotaryEncoder_->getPosition();

        if (newPosition < 0)
        {
            rotaryEncoder_->setPosition(0);
            position = 0;
        }
        else if (newPosition > MAX_POSITION)
        {
            rotaryEncoder_->setPosition(MAX_POSITION);
            position = MAX_POSITION;
        }
        else
        {
            position = static_cast<uint8_t>(newPosition);
        }

        if (position != currentPosition_)
        {
            Serial.printf("%s | Position = %d\n",
                          position > currentPosition_ ? "CW (+)" : "CCW (-)",
                          position);
            currentPosition_ = position;
            if (callback_ != nullptr)
            {
                callback_(currentPosition_);
            }
        }
    }

    long Encoder::getPosition()
    {
        if (!rotaryEncoder_)
        {
            return 0;
        }

        return rotaryEncoder_->getPosition();
    }

    void Encoder::resetPosition()
    {
        if (!rotaryEncoder_)
        {
            return;
        }

        rotaryEncoder_->setPosition(0);
    }

    void Encoder::setCallback(EventCallback cb)
    {
        callback_ = cb;
    }
} // namespace encoder