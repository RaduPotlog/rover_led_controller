#ifndef LED_CONTROLLER_HPP_
#define LED_CONTROLLER_HPP_

#include <Arduino.h>
#include <FastLED.h>

#include "modbus_controller.hpp"

namespace rover_modbus_tcp_led_controller
{

#define NUM_LEDS 24
#define DATA_PIN 13
#define CLOCK_PIN 14

class LedController 
{

public:

    explicit LedController(ModbusController& modbus_controller);

    virtual ~LedController();

    bool pool();
    
private:

    ModbusController * modbus_controller_;

    CRGB leds_[NUM_LEDS];

    uint16_t led_buff_temp[NUM_LEDS * 2] = {0};
};

} // namespace rover_modbus_tcp_led_controller

#endif