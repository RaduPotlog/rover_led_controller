#include "led_controller.hpp"

namespace rover_modbus_tcp_led_controller
{

LedController::LedController()
{
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

void LedController::update(const uint32_t *led_buff) 
{
    uint32_t buff_index = 1UL;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds_[i] = CRGB(led_buff[buff_index] & 0x00FFFFFFUL);
        buff_index++;
    }
    
    FastLED.show();
}

bool LedController::pool()
{
    return true;
}

} // namespace rover_modbus_tcp_led_controller