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

ModbusMessage ModbusController::ReadMultipleRegisters_FC_0x03(ModbusMessage request) 
{
    ModbusMessage response;

    response.setError(request.getServerID(), request.getFunctionCode(), SUCCESS);

    return response;
}

ModbusMessage ModbusController::WriteMultipleRegisters_FC_0x10(ModbusMessage request) 
{
    ModbusMessage response;

    if (request.getFunctionCode() != WRITE_MULT_REGISTERS) {
        response.setError(request.getServerID(), request.getFunctionCode(), FC_MISMATCH);
        return response;
    }

    uint16_t addr = 0;           
    uint16_t words = 0;
    request.get(2, addr);
    request.get(4, words);

    if ((addr + words) > (sizeof(memo) / sizeof(memo[0]))) {
        response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
        return response;
    }

    const uint8_t *data_ptr = request.data();

    memcpy(&memo[addr], &data_ptr[7], words * sizeof(uint16_t));

    response.setError(request.getServerID(), request.getFunctionCode(), SUCCESS);

    return response;
}

void ModbusController::init(ConnectorInterface *connector_interface)
{
    if (is_initialized_) {
        return;
    }

    connector_interface_ = connector_interface;

    modbus_server_wifi_.registerWorker(1, WRITE_MULT_REGISTERS, write_multiple_registers_worker_);
    modbus_server_wifi_.registerWorker(1, READ_HOLD_REGISTER, read_multiple_registers_worker_);

    is_initialized_ = true;
}

bool ModbusController::pool()
{
    if (is_initialized_ == false) {
        return false;
    }
    
    if (!connector_interface_) {
        return false;
    }

    if (!connector_interface_->is_connected()) {
        return (is_interface_connected_ = false);
    }

    if (is_interface_connected_ == false) {
        Serial.println("Start modbus server...");
        modbus_server_wifi_.start(502, 1, 3600000);
        is_interface_connected_ = true;
        Serial.println("Modbus server started!");
    }

    return true;
}

bool ModbusController::read_registers(const uint16_t address, uint16_t *buff, const size_t size)
{
    if (is_initialized_ == false) {
        return false;
    }
    
    if (is_interface_connected_ == false) {
        return false;
    }
    
    memcpy(buff, &memo[address], size * sizeof(uint16_t));

    return true;
}

bool ModbusController::is_connected()
{
    return is_interface_connected_;
}

bool ModbusController::is_client_connected()
{
    if (modbus_server_wifi_.activeClients() > 0) {
        return true;
    }

    return false;
}

} // namespace rover_modbus_tcp_led_controller