#include "uart.hpp"
#include <Arduino.h>
#include <STM32FreeRTOS.h>

namespace uart
{
    void UartHandler::init(uint8_t rxPin, uint8_t txPin)
    {
        SerialVCP_.emplace(rxPin, txPin);
        Serial.begin(115200);
        SerialVCP_->begin(115200);
        delay(3000);
        Serial.println("== UART initialized ==");
        messageQueue_ = xQueueCreate(QUEUE_LENGTH, sizeof(TxCommand));
    }

    void UartHandler::setCallback(CommandCallback cb)
    {
        callback_ = cb;
    }

    void UartHandler::sendCommandToQueue(TxCommand cmd)
    {
        xQueueSend(messageQueue_, &cmd, portMAX_DELAY);
    }

    void UartHandler::checkMessagesTask(void *pvParams)
    {
        auto *self = static_cast<UartHandler *>(pvParams);
        self->checkMessagesLoop();
    }

    Command UartHandler::parseCommand(const char *str)
    {
        if (strcmp(str, "PRESS") == 0)
        {
            return Command::ButtonPress;
        }
        if (strcmp(str, "RELEASE") == 0)
        {
            return Command::ButtonRelease;
        }
        return Command::Unknown;
    }

    void UartHandler::checkMessagesLoop()
    {
        if (!SerialVCP_)
        {
            return;
        }

        while (true)
        {
            while (SerialVCP_->available() > 0)
            {
                char receivedChar = SerialVCP_->read();

                if (receivedChar == '\r')
                {
                    continue;
                }

                if (receivedChar == '\n')
                {
                    buffer_[bufIndex_] = '\0';

                    if (bufIndex_ > 0)
                    {
                        Command cmd = parseCommand(buffer_);
                        if (cmd != Command::Unknown && callback_ != nullptr)
                        {
                            callback_(cmd);
                        }
                    }

                    bufIndex_ = 0;
                }
                else if (bufIndex_ < MAX_MSG_LEN - 1)
                {
                    buffer_[bufIndex_++] = receivedChar;
                }
                else
                {
                    bufIndex_ = 0;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    void UartHandler::sendMessagesTask(void *pvParams)
    {
        auto *self = static_cast<UartHandler *>(pvParams);
        self->sendMessagesLoop();
    }
    void UartHandler::sendMessagesLoop()
    {
        TxCommand command;
        while (true)
        {
            if (xQueueReceive(messageQueue_, &command, portMAX_DELAY) == pdPASS)
            {

                const char *stringType;
                switch (command.type)
                {
                case Type::On:
                    stringType = "On";
                    break;
                case Type::Off:
                    stringType = "Off";
                    break;
                case Type::R:
                    stringType = "R";
                    break;
                case Type::G:
                    stringType = "G";
                    break;
                case Type::B:
                    stringType = "B";
                    break;
                default:
                    stringType = "Unknown";
                    break;
                }
                Serial.printf("got type=%s value=%d\n",
                              stringType,
                              command.value);

                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
}
