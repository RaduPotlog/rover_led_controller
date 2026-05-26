#include <Arduino.h>

#include <WiFi.h>

#include "connector_interface.hpp"
#include "wifi_connector.hpp"
#include "modbus_controller.hpp"
#include "led_controller.hpp"
#include "udp_connection.hpp"

const String ssid = "TP-Link_30C0";
const String pass = "39063106";
const int port = 3333;

uint32_t red_color_led_buff[NUM_LEDS];
uint32_t blue_color_led_buff[NUM_LEDS];
uint32_t cleared_color_led_buff[NUM_LEDS];

rover_modbus_tcp_led_controller::ConnectorInterface *gWifiConnectionInterface = nullptr;
rover_modbus_tcp_led_controller::LedController gLedController;
rover_modbus_tcp_led_controller::UdpConnection  gUdpConnection;

static bool is_fisrst_time_connected = false;
    
void setup()
{
    Serial.begin(115200);
  
    while (!Serial) { ; }

    Serial.println("UDP Server LED");
    
    for (int i = 0; i < NUM_LEDS; i++) {
        red_color_led_buff[i] = 0x00FF0000UL;
        cleared_color_led_buff[i] = 0x00000000UL;
        blue_color_led_buff[i] = 0x000000FFUL;
    }

    gWifiConnectionInterface = &rover_modbus_tcp_led_controller::WifiConnector::getInstance();
    gWifiConnectionInterface->connect(ssid, pass);
    gUdpConnection.init(gWifiConnectionInterface, port, [](AsyncUDPPacket packet) {
        
        if (is_fisrst_time_connected == false) {
            return;
        }

        if (packet.length() < (NUM_LEDS * 2 * sizeof(uint16_t) + 8)) {
            Serial.println("Received packet with invalid length");
            return;
        }

        gLedController.update(reinterpret_cast<const uint32_t *>(packet.data() + sizeof(uint32_t)));
    });
}

void loop()
{
    static uint32_t gpt0 = 20UL;
    static bool is_red_light = true;

    static uint32_t *ptr_temp = cleared_color_led_buff;
    
    if (gWifiConnectionInterface != nullptr) {
        
        if (!gWifiConnectionInterface->is_connected()) {
            
            if (gpt0 > 0) {
                gpt0--;
            } else {
                if (is_red_light) {
                    is_red_light = false;
                    ptr_temp = cleared_color_led_buff;
                } else {
                    is_red_light = true;
                    ptr_temp = red_color_led_buff;
                }

                gpt0 = 20UL;
            }

            is_fisrst_time_connected = false;
            gLedController.update(ptr_temp);
        } else {
            gpt0 = 20UL;
            is_red_light = true;
            ptr_temp = red_color_led_buff;
            
            if (is_fisrst_time_connected == false) {
                gLedController.update(blue_color_led_buff);
                is_fisrst_time_connected = true;
            }
        }

        (void)gLedController.pool();
        (void)gUdpConnection.pool();
    }

    delay(100);
}