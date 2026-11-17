#pragma once

#include <cstdint>
#include <stddef.h>
#include <STM32FreeRTOS.h>
#include <optional>
#include <Arduino.h>

namespace uart
{
    enum class RxCommand
    {
        ButtonPress,
        ButtonRelease,
        Unknown,
    };

    using CommandCallback = void (*)(RxCommand);

    enum class CommandType
    {
        ON,
        OFF,
        R,
        G,
        B,
        Unknown
    };

    struct TxCommand
    {
        CommandType type;
        uint8_t value;
    };

    class UartHandler
    {
    public:
        void init(uint8_t rxPin, uint8_t txPin);
        static void checkMessagesTask(void *pvParams);
        static void sendMessagesTask(void *pvParams);
        void setCallback(CommandCallback cb);
        void sendCommandToQueue(TxCommand cmd);

    private:
        void checkMessagesLoop();
        void sendMessagesLoop();
        RxCommand parseCommand(const char *str);

        std::optional<Uart> SerialVCP_;

        static constexpr size_t MAX_MSG_LEN = 32;
        char buffer_[MAX_MSG_LEN]{};
        size_t bufIndex_ = 0;

        QueueHandle_t messageQueue_ = nullptr;
        static constexpr UBaseType_t QUEUE_LENGTH = 10;

        CommandCallback callback_ = nullptr;
    };

} // namespace uart