#ifndef MODBUS_CONTROLLER_HPP_
#define MODBUS_CONTROLLER_HPP_

#include <ArduinoModbus.h>

#include "connector_interface.hpp"

namespace rover_modbus_tcp_led_controller
{

class ModbusController
{

public:

    /// @brief 
    ModbusController();

    /// @brief 
    virtual ~ModbusController();

    /// @brief 
    /// @param connector_interface 
    void init(ConnectorInterface *connector_interface);

    /// @brief 
    /// @return 
    bool pool();

    /// @brief 
    /// @return 
    long read_registers(const uint16_t address);
    
    /// @brief 
    /// @return 
    bool is_connected();
    
private:

    /// @brief 
    ConnectorInterface *connector_interface_;

    /// @brief 
    ModbusTCPServer modbus_tcp_server_;

    /// @brief 
    bool is_started_{false};

    /// @brief
    bool is_client_accepted_{false};
};

} // namespace rover_modbus_tcp_led_controller

#endif