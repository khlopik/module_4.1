#pragma once

#include <STM32FreeRTOS.h>
#include <optional>
#include <EasyButton.h>

namespace button
{
    using EventCallback = void (*)();

    class ButtonHandler
    {
    public:
        void init(uint8_t pin);
        void update();
        void setCallback(EventCallback cb);
        void setCallbackToBuiltInButton(EventCallback cb);

    private:
        std::optional<EasyButton> button_;
        std::optional<EasyButton> builtInButton_;
        EventCallback callback_ = nullptr;
        EventCallback callbackToBuiltIn_ = nullptr;
    };
}