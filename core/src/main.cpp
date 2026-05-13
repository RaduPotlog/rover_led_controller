#include <Arduino.h>

#include <WiFi.h>

#include "connector_interface.hpp"
#include "wifi_connector.hpp"
#include "modbus_controller.hpp"
#include "led_controller.hpp"

const String ssid = "MikroLAB";
const String pass = "@2wsxdr%5";
unsigned short port = 502;

rover_modbus_tcp_led_controller::ConnectorInterface *gPtrModbusConnectionInterface = nullptr;
rover_modbus_tcp_led_controller::ModbusController gModbusController;
rover_modbus_tcp_led_controller::LedController gLedController(gModbusController);

void setup()
{
    Serial.begin(115200);
  
    // Wait for serial port to connect. Needed for native USB
    while (!Serial) { ; }

    Serial.println("Modbus TCP Server LED");
  
    // Connect to modbus controller
    gPtrModbusConnectionInterface = &rover_modbus_tcp_led_controller::WifiConnector::getInstance();
    gPtrModbusConnectionInterface->connect(port, ssid, pass);
    gModbusController.init(gPtrModbusConnectionInterface);
}

void loop()
{
    if (gPtrModbusConnectionInterface != nullptr) {
        // Pool controllers
        (void)gPtrModbusConnectionInterface->pool();
        (void)gModbusController.pool();
        (void)gLedController.pool();
    }
}