#include "led_controller.hpp"

namespace rover_modbus_tcp_led_controller
{

LedController::LedController()
{
    // modbus_controller_ = &modbus_controller;

    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds_, NUM_LEDS);
}

LedController::~LedController()
{

}

void LedController::update(const uint16_t *led_buff) 
{
    uint32_t led_state = 0;
    uint32_t led_index = 0;

    for (int i = 0; i < NUM_LEDS * 2; i = i + 2) {
        led_state  = static_cast<uint32_t>(led_buff[i] << 16);
        led_state |= static_cast<uint32_t>(led_buff[i + 1]);
        led_state &= 0x00FFFFFFU;
        leds_[led_index] = CRGB(led_state);
        led_index++;
    }
    
    FastLED.show();
}

bool LedController::pool()
{
    // if (!modbus_controller_) {
    //     return false;
    // }

    // uint32_t led_state = 0;

    // if (!modbus_controller_->is_connected()) {

    //     for (int i = 0; i < NUM_LEDS; i++) {
    //         led_state = 0x00FF0000U;
    //         leds_[i] = CRGB(led_state);
    //     }

    //     FastLED.show();
    //     return false;
    // }

    // if (!modbus_controller_->is_client_connected()) {
    //     for (int i = 0; i < NUM_LEDS; i++) {
    //         led_state = 0x0000FF00U;
    //         leds_[i] = CRGB(led_state);
    //     }

    //     FastLED.show();
    //     return false;
    // }

    // if (!modbus_controller_->read_registers(2, led_buff_temp, sizeof(led_buff_temp) / sizeof(led_buff_temp[0]))) {
    //     return false;
    // }

    // uint32_t led_index = 0;

    // for (int i = 0; i < sizeof(led_buff_temp) / sizeof(led_buff_temp[0]); i = i + 2) {
    //     led_state  = static_cast<uint32_t>(led_buff_temp[i] << 16);
    //     led_state |= static_cast<uint32_t>(led_buff_temp[i + 1]);
    //     led_state = led_state & 0x00FFFFFFU;
    //     leds_[led_index] = CRGB(led_state);
    //     led_index++;
    // }
    
    // FastLED.show();
    
    return true;
}

} // namespace rover_modbus_tcp_led_controller