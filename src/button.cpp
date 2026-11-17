#include "button.hpp"
#include "config.hpp"
#include <STM32FreeRTOS.h>

namespace button
{
    void ButtonHandler::init(uint8_t pin)
    {
        button_.emplace(pin);
        builtInButton_.emplace(buttonConfig::builtInPin);
        button_->begin();
        builtInButton_->begin();
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

    void ButtonHandler::setCallbackToBuiltInButton(EventCallback cb)
    {
        if (!builtInButton_)
        {
            return;
        }

        callbackToBuiltIn_ = cb;
        builtInButton_->onPressed(callbackToBuiltIn_);
    }

    void ButtonHandler::update()
    {
        if (button_)
        {
            button_->read();
        }

        if (builtInButton_)
        {
            builtInButton_->read();
        }
    }
}