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
  bool isRGBLedOn = false;

  void onEspCommand(uart::RxCommand cmd)
  {
    Serial.println("== onEspCommand ==");
    switch (cmd)
    {
    case uart::RxCommand::ButtonPress:
      ledHandler.turnOnBuiltInLed();
      break;
    case uart::RxCommand::ButtonRelease:
      ledHandler.turnOffBuiltInLed();
      break;
    default:
      break;
    }
  }

  void onEncoderUpdate(uint8_t value)
  {
    Serial.println("== onEncoderUpdate ==");
    uart::TxCommand commandToSend;

    switch (ledHandler.getCurrentColor())
    {
    case led::Color::Red:
      commandToSend.type = uart::CommandType::R;
      break;
    case led::Color::Green:
      commandToSend.type = uart::CommandType::G;
      break;
    case led::Color::Blue:
      commandToSend.type = uart::CommandType::B;
      break;
    default:
      commandToSend.type = uart::CommandType::Unknown;
      break;
    }

    commandToSend.value = (value == 0) ? 0 : (value * 16 - 1);
    uartHandler.sendCommandToQueue(commandToSend);
  }

  void onBuiltInButtonClick()
  {
    Serial.println("== onBuiltInButtonClick ==");
    uart::TxCommand commandToSend;
    commandToSend.value = 1;
    if (isRGBLedOn)
    {
      commandToSend.type = uart::CommandType::OFF;
      isRGBLedOn = false;
    }
    else
    {
      commandToSend.type = uart::CommandType::ON;
      isRGBLedOn = true;
    }
    uartHandler.sendCommandToQueue(commandToSend);
  }
}

void inputTask(void *)
{
  while (true)
  {
    buttonHandler.update();
    encoderHandler.update();
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void setup()
{
  uartHandler.init(uartConfig::rxPin, uartConfig::txPin);
  uartHandler.setCallback(&onEspCommand);
  ledHandler.init(ledConfig::pinRed, ledConfig::pinGreen, ledConfig::pinBlue);
  buttonHandler.init(buttonConfig::pin);
  buttonHandler.setCallback([]()
                            { ledHandler.switchColor(); });
  buttonHandler.setCallbackToBuiltInButton(onBuiltInButtonClick);
  encoderHandler.init(encoderConfig::clkPin, encoderConfig::dtPin);
  encoderHandler.setCallback(&onEncoderUpdate);

  if (xTaskCreate(uartHandler.checkMessagesTask, "Check messages", 256, &uartHandler, 3, NULL) != pdPASS)
  {
    abort();
  };

  if (xTaskCreate(uartHandler.sendMessagesTask, "Send messages", 256, &uartHandler, 3, NULL) != pdPASS)
  {
    abort();
  }

  if (xTaskCreate(inputTask, "Input", 256, NULL, 2, NULL) != pdPASS)
  {
    abort();
  }

  vTaskStartScheduler();
}

void loop()
{
}
