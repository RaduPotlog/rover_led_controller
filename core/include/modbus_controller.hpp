#ifndef MODBUS_CONTROLLER_HPP_
#define MODBUS_CONTROLLER_HPP_

#include <ModbusServerWiFi.h>

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
    bool read_registers(const uint16_t address, uint16_t *buff, const size_t size);
    
    /// @brief 
    /// @return 
    bool is_connected();
    
    /// @brief 
    /// @return 
    bool is_client_connected();

private:

    /// @brief 
    /// @param request 
    /// @return 
    ModbusMessage ReadMultipleRegisters_FC_0x03(ModbusMessage request);

    /// @brief 
    /// @param request 
    /// @return 
    ModbusMessage WriteMultipleRegisters_FC_0x10(ModbusMessage request);

    /// @brief 
    MBSworker write_multiple_registers_worker_ = std::bind(&ModbusController::WriteMultipleRegisters_FC_0x10, this, std::placeholders::_1);
    
    /// @brief 
    MBSworker read_multiple_registers_worker_ = std::bind(&ModbusController::ReadMultipleRegisters_FC_0x03, this, std::placeholders::_1);

    /// @brief 
    ConnectorInterface *connector_interface_;

    ModbusServerTCP<WiFiServer, WiFiClient> modbus_server_wifi_;

    /// @brief 
    bool is_initialized_{false};

    /// @brief
    bool is_interface_connected_{false};

    uint16_t memo[256];
};

} // namespace rover_modbus_tcp_led_controller

#endif