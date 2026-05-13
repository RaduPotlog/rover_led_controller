#ifndef CONNECTION_INTERFACE_HPP
#define CONNECTION_INTERFACE_HPP

#include <WString.h>
#include <WiFi.h>

namespace rover_modbus_tcp_led_controller
{

/// @brief Interface for modbus network connections.
class ConnectorInterface
{

public:

    /// @brief Connect to the network.
    virtual void connect() = 0;
    
    /// @brief Connect to the network with specific parameters.
    /// @param port 
    /// @param ssid 
    /// @param pass 
    virtual void connect(const unsigned short port, const String& ssid, const String& pass) = 0;

    /// @brief Disconnect from the network.
    virtual void disconnect() = 0;
    
    /// @brief Check if the client connection is active.
    /// @return true if connected, false otherwise.
    virtual bool is_client_connected() = 0;

    /// @brief Pool connection stack
    /// @return
    virtual bool pool() = 0;

    /// @brief 
    /// @return 
    virtual WiFiClient& get_client() = 0;
};

} // namespace rover_modbus_tcp_led_controller

#endif // CONNECTION_INTERFACE_HPP