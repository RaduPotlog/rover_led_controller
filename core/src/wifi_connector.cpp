#include "wifi_connector.hpp"

namespace rover_modbus_tcp_led_controller
{

bool WifiConnector::is_wifi_connected_ = false;

WifiConnector& WifiConnector::getInstance()
{
    static WifiConnector instance;
    return instance;
}

void WifiConnector::station_connected_callback(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to router successfully!");
}

void WifiConnector::got_ip_callback(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    is_wifi_connected_ = true;
}

void WifiConnector::disconnected_callback(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");

    is_wifi_connected_ = false;
    WiFi.reconnect();
}

void WifiConnector::print_status() 
{
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void WifiConnector::connect()
{

}

void WifiConnector::connect(
    const String& ssid, 
    const String& pass)
{
    ssid_ = ssid;
    pass_ = pass;

    WiFi.onEvent(station_connected_callback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(got_ip_callback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(disconnected_callback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.mode(WIFI_STA);
    
    IPAddress local_ip(192, 168, 99, 101);
    IPAddress gateway(192, 168, 99, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primary_dns(8, 8, 8, 8);
    IPAddress secondary_dns(8, 8, 4, 4);

    if (!WiFi.config(local_ip, gateway, subnet, primary_dns, secondary_dns)) {
        Serial.println("STA Failed to configure");
        return;
    }

    WiFi.begin(ssid_, pass_);
}

void WifiConnector::disconnect()
{
    WiFi.disconnect();
}

bool WifiConnector::is_connected()
{   
    return is_wifi_connected_;
}

} // namespace rover_modbus_tcp_led_controller