#include "encoder.hpp"
#include "RotaryEncoder.h"

namespace encoder
{
    void Encoder::init(uint8_t clkPin, uint8_t dtPin)
    {
        rotaryEncoder_.emplace(dtPin, clkPin, RotaryEncoder::LatchMode::FOUR3);
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

        Serial.println("== ENCODER: update() ==");
        uint8_t position;
        const long newPosition = rotaryEncoder_->getPosition();
        Serial.printf("New position: %ld\n", newPosition);

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

        Serial.printf("position: %d\n", position);
        Serial.printf("currentPosition: %d\n", currentPosition_);

        if (position != currentPosition_)
        {
            Serial.printf("%s | Position = %d\n",
                          position > currentPosition_ ? "CW (+)" : "CCW (-)",
                          position);
            Serial.println("After loggin position");
            currentPosition_ = position;
            if (callback_ != nullptr)
            {
                Serial.println("Running callback");
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

    void Encoder::setPosition(uint8_t position)
    {
        if (!rotaryEncoder_)
        {
            return;
        }

        currentPosition_ = position;
        rotaryEncoder_->setPosition(position);
    }

    void Encoder::setCallback(EventCallback cb)
    {
        callback_ = cb;
    }
} // namespace encoder