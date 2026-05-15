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

rover_modbus_tcp_led_controller::ConnectorInterface *gWifiConnectionInterface = nullptr;
rover_modbus_tcp_led_controller::LedController gLedController;
rover_modbus_tcp_led_controller::UdpConnection  gUdpConnection;

void setup()
{
    Serial.begin(115200);
  
    while (!Serial) { ; }

    Serial.println("Modbus TCP Server LED");
  
    gWifiConnectionInterface = &rover_modbus_tcp_led_controller::WifiConnector::getInstance();
    gWifiConnectionInterface->connect(ssid, pass);
    gUdpConnection.init(gWifiConnectionInterface, port, [](AsyncUDPPacket packet) {
        gLedController.update(reinterpret_cast<const uint32_t *>(packet.data()));
    });
}

void loop()
{
    if (gWifiConnectionInterface != nullptr) {
        (void)gLedController.pool();
        (void)gUdpConnection.pool();
    }
}