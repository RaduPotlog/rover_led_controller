#ifndef CONNECTION_WIFI_HPP
#define CONNECTION_WIFI_HPP

#include <WiFi.h>

#include "connector_interface.hpp"

namespace rover_modbus_tcp_led_controller
{

/// @brief Concrete implementation of ConnectorInterface for WiFi connections.
class WifiConnector : public ConnectorInterface
{

public:
    
    /// @brief Get the singleton instance of ModbusConnectionWifi.
    /// @return Reference to the singleton instance.
    static WifiConnector& getInstance();

    /// @brief Destructor for ConnectionWifi.
    virtual ~WifiConnector() = default;
    
    /// @brief Connect to the WiFi network using the stored SSID and password.
    void connect() override;

    /// @brief Connect to the WiFi network using the provided SSID and password.
    void connect(const unsigned short port, const String& ssid, const String& pass) override;
    
    /// @brief Disconnect from the WiFi network.
    void disconnect() override;
    
    /// @brief Check if the WiFi connection is active.
    /// @return true if connected to the WiFi network, false otherwise.
    bool is_client_connected() override;

    /// @brief 
    /// @return 
    WiFiClient& get_client() override;

    /// @brief Pool modbus controller
    /// @return 
    bool pool() override;

private:

    /// @brief Private constructor for the singleton pattern.
    WifiConnector() = default;
    
    /// @brief Deleted copy constructor to prevent copying of the singleton instance.
    WifiConnector(const WifiConnector&) = delete;
    
    /// @brief Deleted assignment operator to prevent copying of the singleton instance.
    WifiConnector& operator=(const WifiConnector&) = delete;
    
    /// @brief Callback for when the station is connected to the WiFi network.
    /// @param event 
    /// @param info 
    static void station_connected_callback(WiFiEvent_t event, WiFiEventInfo_t info);

    /// @brief Callback for when the station gets an IP address after connecting to the WiFi network.
    /// @param event 
    /// @param info 
    static void got_ip_callback(WiFiEvent_t event, WiFiEventInfo_t info);

    /// @brief Callback for when the station is disconnected from the WiFi network.
    /// @param event 
    /// @param info 
    static void disconnected_callback(WiFiEvent_t event, WiFiEventInfo_t info);

    /// @brief Print the current WiFi status, including SSID, IP address, and signal strength.
    static void print_status();

    /// @brief WiFi server used for network communication.
    static WiFiServer wifi_server_;
    
    /// @brief WiFi client used for handling incoming connections.
    WiFiClient wifi_client_;

    /// @brief Port number for the WiFi server.
    unsigned short port_{502};

    /// @brief WiFi network credentials.
    String ssid_{""};

    /// @brief WiFi network password.
    String pass_{""};

    /// @brief Connection status
    bool is_client_connected_{false};
};

} // namespace rover_modbus_tcp_led_controller

#endif // CONNECTION_WIFI_HPP