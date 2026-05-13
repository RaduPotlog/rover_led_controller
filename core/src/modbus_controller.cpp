#include "modbus_controller.hpp"

namespace rover_modbus_tcp_led_controller
{

ModbusController::ModbusController()
: connector_interface_{nullptr}
{

}

ModbusController::~ModbusController()
{

}

void ModbusController::init(ConnectorInterface *connector_interface)
{
    if (is_started_) {
        return;
    }

    connector_interface_ = connector_interface;

    if (modbus_tcp_server_.begin()) {
        is_started_ = true;

        modbus_tcp_server_.configureHoldingRegisters(0x00, 52);
    }
}

bool ModbusController::pool()
{
    if (is_started_ == false) {
        return false;
    }
    
    if (!connector_interface_) {
        return false;
    }

    if (!connector_interface_->is_client_connected()) {
        return (is_client_accepted_ = false);
    }

    if (is_client_accepted_ == false) {
        modbus_tcp_server_.accept(connector_interface_->get_client());
        is_client_accepted_ = true;
        Serial.println("New modbus client accepted!");
    }

    modbus_tcp_server_.poll();

    return true;
}

long ModbusController::read_registers(const uint16_t address)
{
    if (is_client_accepted_ == false) {
        return -1;
    }

    return modbus_tcp_server_.holdingRegisterRead(address);
}

bool ModbusController::is_connected()
{
    return is_client_accepted_;
}

} // namespace rover_modbus_tcp_led_controller