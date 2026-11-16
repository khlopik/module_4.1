#include <Arduino.h>
#include "config.hpp"
#include "button.hpp"
#include "encoder.hpp"
#include "uart.hpp"
#include "led.hpp"
#include <STM32FreeRTOS.h>

namespace
{
  button::ButtonHandler buttonHandler;
  led::LedHandler ledHandler;
  encoder::Encoder encoderHandler;
  uart::UartHandler uartHandler;

  void onEspCommand(uart::Command cmd)
  {
    switch (cmd)
    {
    case uart::Command::ButtonPress:
      ledHandler.turnOnBuiltInLed();
      break;
    case uart::Command::ButtonRelease:
      ledHandler.turnOffBuiltInLed();
      break;
    default:
      break;
    }
  }

  void onEncoderUpdate(uint8_t value)
  {
    uart::TxCommand commandToSend;

    switch (ledHandler.getCurrentColor())
    {
    case led::Color::Red:
      commandToSend.type = uart::Type::R;
      break;
    case led::Color::Green:
      commandToSend.type = uart::Type::G;
      break;
    case led::Color::Blue:
      commandToSend.type = uart::Type::B;
      break;
    default:
      commandToSend.type = uart::Type::Unknown;
      break;
    }

    commandToSend.value = value;
    uartHandler.sendCommandToQueue(commandToSend);
  }
}

void setup()
{
  ledHandler.init(ledConfig::pinRed, ledConfig::pinGreen, ledConfig::pinBlue);
  buttonHandler.init(buttonConfig::pin);
  buttonHandler.setCallback([]()
                            { ledHandler.switchColor(); });
  encoderHandler.init(encoderConfig::clkPin, encoderConfig::dtPin);
  encoderHandler.setCallback(&onEncoderUpdate);
  uartHandler.init(uartConfig::rxPin, uartConfig::txPin);
  uartHandler.setCallback(&onEspCommand);

  if (xTaskCreate(uartHandler.checkMessagesTask, "Check messages", 2048, NULL, 3, NULL) != pdPASS)
  {
    abort();
  };

  if (xTaskCreate(uartHandler.sendMessagesTask, "Send messages", 2048, NULL, 3, NULL) != pdPASS)
  {
    abort();
  }

  vTaskStartScheduler();
}

void loop()
{
  buttonHandler.update();
  encoderHandler.update();
  vTaskDelay(pdMS_TO_TICKS(5));
}
