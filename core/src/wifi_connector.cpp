#include "wifi_connector.hpp"

namespace rover_modbus_tcp_led_controller
{

// ModbusTCPServer WifiConnector::modbus_tcp_server_;
WiFiServer WifiConnector::wifi_server_;

WifiConnector& WifiConnector::getInstance()
{
    static WifiConnector instance;
    return instance;
}

void WifiConnector::station_connected_callback(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to router successfully!");
    wifi_server_.begin(502);
}

void WifiConnector::got_ip_callback(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WifiConnector::disconnected_callback(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");

    wifi_server_.stop();
    wifi_server_.close();
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
    connect(port_, ssid_, pass_);
}

void WifiConnector::connect(
    const unsigned short port, 
    const String& ssid, 
    const String& pass)
{
    port_ = port;
    ssid_ = ssid;
    pass_ = pass;

    WiFi.onEvent(station_connected_callback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(got_ip_callback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(disconnected_callback, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.mode(WIFI_STA);
    
    if (WL_CONNECTED == WiFi.begin(ssid_, pass_)) {
        Serial.println("Connected to WiFi!");
    } else {
        Serial.println("Failed to connect to WiFi!");
    }
}

void WifiConnector::disconnect()
{

}

bool WifiConnector::is_client_connected()
{   
    return is_client_connected_;
}

WiFiClient& WifiConnector::get_client() 
{
    return wifi_client_;
}

bool WifiConnector::pool() 
{
    if (is_client_connected_ == false) {
        wifi_client_ = wifi_server_.available();
        
        if (!wifi_client_) {
            return (is_client_connected_ = false);
        }
        
        is_client_connected_ = true;
        Serial.println("New client connected!");
    }
    
    if (!wifi_client_) {
        is_client_connected_ = false;
    }

    return true;
}

} // namespace rover_modbus_tcp_led_controller