
#include "SerialPorts.h"

#include <libserialport.h>
#include <stdexcept>
#include <vector>

/******************************************************************************/
/* Serial Port ****************************************************************/
SerialPort::SerialPort(sp_port* port)
    : m_SerialPort{port}
{
}

SerialPort::~SerialPort()
{
    sp_free_port(m_SerialPort);
}

SerialPort::SerialPort(const SerialPort& other)
    : m_SerialPort{nullptr}
{
    sp_return r_copyport = sp_copy_port(other.m_SerialPort, &m_SerialPort);
    if (r_copyport != SP_OK) throw std::runtime_error("Failed to copy SerialPort.");
}

SerialPort::SerialPort(SerialPort&& other)
    : m_SerialPort{other.m_SerialPort}
{
    other.m_SerialPort = nullptr;
}

SerialPort& SerialPort::operator=(const SerialPort& other)
{
    if (&other != this) // (this) and (other) are different instances
    {
        sp_return r_copyport = sp_copy_port(other.m_SerialPort, &m_SerialPort);
        if (r_copyport != SP_OK) throw std::runtime_error("Failed to copy SerialPort.");
    }
    return *this;
}

SerialPort& SerialPort::operator=(SerialPort&& other)
{
    if (&other != this) // (this) and (other) are different instances
    {
        m_SerialPort = other.m_SerialPort;
        other.m_SerialPort = nullptr;
    }
    return *this;
}

std::string SerialPort::name() const
{
    return sp_get_port_name(m_SerialPort);
}
/******************************************************************************/

/******************************************************************************/
/* SerialPorts ****************************************************************/
SerialPorts::SerialPortList SerialPorts::ListAvailableSerialPorts()
{
    // Get null-terminated list of available serial ports (libserialport).
    sp_port** port_list;
    sp_return r_listports = sp_list_ports(&port_list);
    if (r_listports != SP_OK) throw std::runtime_error(
        "Failed to retrieve list of available serial ports from libserialport.");

    // Calculate number of available serial ports found.
    size_t num_ports = 0;
    while (port_list[num_ports] != nullptr) num_ports += 1;

    // Generate list of available serial ports as type SerialPort
    std::vector<SerialPort> available_ports;
    available_ports.reserve(num_ports);
    for (size_t i = 0; i < num_ports; ++i) {
        sp_port* port_copy;
        sp_return r_copyport = sp_copy_port(port_list[0], &port_copy);
        if (r_copyport != SP_OK) throw std::runtime_error(
            "Failed to construct list of available serial ports as type SerialPort.");
        available_ports.emplace_back(port_copy);
    }

    sp_free_port_list(port_list);
    return available_ports;
}
/******************************************************************************/
