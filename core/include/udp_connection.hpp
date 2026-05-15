#ifndef CONNECTION_UDP_HPP
#define CONNECTION_UDP_HPP

#include <AsyncUDP.h>

#include "connector_interface.hpp"

namespace rover_modbus_tcp_led_controller
{

/// @brief Concrete implementation of ConnectorInterface for UDP connections.
class UdpConnection
{

public:
    
    /// @brief Private constructor.
    explicit UdpConnection();
    
    /// @brief Deleted copy constructor to prevent copying of the singleton instance.
    UdpConnection(const UdpConnection&) = delete;
    
    /// @brief Destructor for UdpConnection.
    virtual ~UdpConnection() =  default;
    
    /// @brief 
    /// @return 
    bool init(ConnectorInterface *connector_interface, const int port, AuPacketHandlerFunction cb);

    /// @brief 
    /// @return 
    bool pool();

    /// @brief Check if the UDP connection is active.
    /// @return true if connected to the UDP endpoint, false otherwise.
    bool is_connected();

private:

    /// @brief Deleted assignment operator to prevent copying of the singleton instance.
    UdpConnection& operator=(const UdpConnection&) = delete;

    /// @brief Callback for when a UDP packet is received.
    /// @param packet The received UDP packet.
    AuPacketHandlerFunction data_recv_handler_;

    /// @brief 
    ConnectorInterface *connector_interface_;

    /// @brief AsyncUDP instance for managing the UDP connection.
    AsyncUDP udp_;

    /// @brief UDP port.
    uint16_t port_{0};

    /// @brief 
    bool is_initialized_{false};

    /// @brief
    bool is_interface_connected_{false};
};

} // namespace rover_modbus_tcp_led_controller

#endif // CONNECTION_UDP_HPP