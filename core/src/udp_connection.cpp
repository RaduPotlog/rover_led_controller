#include "udp_connection.hpp"

namespace rover_modbus_tcp_led_controller
{

UdpConnection::UdpConnection()
: connector_interface_{nullptr}
, port_(3333)
{
    
}

bool UdpConnection::init(
    ConnectorInterface *connector_interface, 
    const int port,
    AuPacketHandlerFunction cb)
{
    connector_interface_ = connector_interface;
    port_ = port;
    is_initialized_ = true;
    data_recv_handler_ = cb;

    return true;
}

bool UdpConnection::pool() 
{
    if (is_initialized_ == false) {
        return false;
    }
    
    if (!connector_interface_) {
        return false;
    }

    if (!connector_interface_->is_connected()) {
        if (udp_.connected()) {
            udp_.close();
            Serial.println("UDP connection closing...");
        }

        return (is_interface_connected_ = false);
    }

    if (is_interface_connected_ == false) {
        Serial.println("Start udp listener...");
        
        if (udp_.listen(port_)) {
            Serial.println("UDP listening on port " + String(port_) + "...");
            
            udp_.onPacket([this](AsyncUDPPacket packet) {
                if (data_recv_handler_) {
                    data_recv_handler_(packet);
                }
            });

            Serial.println("UDP listening on port " + String(port_) + " started successfully!");
            is_interface_connected_ = true;
        } else {
            Serial.println("Failed to start UDP listener");
            return (is_interface_connected_ = false);
        }
    }

    return true;
}

bool UdpConnection::is_connected() 
{
    return is_interface_connected_;
}

} // namespace rover_modbus_tcp_led_controller


