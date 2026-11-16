#include "button.hpp"
#include "config.hpp"
#include <STM32FreeRTOS.h>

namespace button
{
    void ButtonHandler::init(uint8_t pin)
    {
        button_.emplace(pin);
        button_->begin();
    }

    void ButtonHandler::setCallback(EventCallback cb)
    {
        if (!button_)
        {
            return;
        }

        callback_ = cb;
        button_->onPressed(callback_);
    }

    void ButtonHandler::update()
    {
        if (!button_)
        {
            return;
        }

        button_->read();
    }
}