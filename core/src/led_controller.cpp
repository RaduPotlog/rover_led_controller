#include "led_controller.hpp"

namespace rover_modbus_tcp_led_controller
{

LedController::LedController(ModbusController& modbus_controller)
{
    modbus_controller_ = &modbus_controller;

    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds_, NUM_LEDS);
}

LedController::~LedController()
{

}

bool LedController::pool() 
{
    if (!modbus_controller_) {
        return false;
    }

    uint32_t led_state = 0;

    if (!modbus_controller_->is_connected()) {

        for (int i = 0; i < NUM_LEDS; i++) {
            led_state = 0x00FF0000U;
            leds_[i] = CRGB(led_state);
        }

        FastLED.show();
        return false;
    }

    uint16_t reg_index = 2;
    long holding_register_value = 0;
    bool led_show = true;

    for (int i = 0; i < NUM_LEDS; i++) {
        holding_register_value = modbus_controller_->read_registers(reg_index + 1);
        led_state = static_cast<uint32_t>(holding_register_value);
        holding_register_value = modbus_controller_->read_registers(reg_index);
        if (holding_register_value < 0) {
            led_show = false;
            break;
        }

        led_state |= static_cast<uint32_t>(holding_register_value << 16);
        led_state = led_state & 0x00FFFFFFU;
        leds_[i] = CRGB(led_state);
        reg_index += 2;
    }
    
    if (led_show) {
      FastLED.show();
    }

    return led_show;
}

} // namespace rover_modbus_tcp_led_controller