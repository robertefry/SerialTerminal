
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

SerialPort::TransportType SerialPort::transport_type() const
{
    switch (sp_get_port_transport(m_SerialPort))
    {
        case SP_TRANSPORT_NATIVE:    return SerialPort::TRANSPORT_NATIVE;
        case SP_TRANSPORT_USB:       return SerialPort::TRANSPORT_USB;
        case SP_TRANSPORT_BLUETOOTH: return SerialPort::TRANSPORT_BLUETOOTH;
        default: return SerialPort::TRANSPORT_UNKNOWN;
    }
}

std::string SerialPort::name() const
{
    try {
        return sp_get_port_name(m_SerialPort);
    } catch (const std::exception&) {
        return "Failed to retrieve";
    }
}

std::string SerialPort::description() const
{
    try {
        return sp_get_port_description(m_SerialPort);
    } catch (const std::exception&) {
        return "Failed to retrieve";
    }
}

std::string SerialPort::transport() const
{
    switch (transport_type())
    {
        case TRANSPORT_NATIVE:    return "Native";
        case TRANSPORT_USB:       return "USB";
        case TRANSPORT_BLUETOOTH: return "Bluetooth";
        default: return "Unknown";
    }
}

std::string SerialPort::manufacturer() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            return sp_get_port_usb_manufacturer(m_SerialPort);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::product() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            return sp_get_port_usb_product(m_SerialPort);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::serial() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            return sp_get_port_usb_serial(m_SerialPort);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::vendor_id() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            int vid, pid;
            sp_return r_get = sp_get_port_usb_vid_pid(m_SerialPort, &vid, &pid);
            if (r_get != SP_OK) throw std::runtime_error("Failed to get serial device vendor id.");
            return std::to_string(vid);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::product_id() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            int vid, pid;
            sp_return r_get = sp_get_port_usb_vid_pid(m_SerialPort, &vid, &pid);
            if (r_get != SP_OK) throw std::runtime_error("Failed to get serial device product id.");
            return std::to_string(pid);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::usb_bus() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            int bus, adr;
            sp_return r_get = sp_get_port_usb_bus_address(m_SerialPort, &bus, &adr);
            if (r_get != SP_OK) throw std::runtime_error("Failed to get serial device product id.");
            return std::to_string(bus);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::usb_addr() const
{
    if (transport_type() == TRANSPORT_USB) {
        try {
            int bus, adr;
            sp_return r_get = sp_get_port_usb_bus_address(m_SerialPort, &bus, &adr);
            if (r_get != SP_OK) throw std::runtime_error("Failed to get serial device product id.");
            return std::to_string(adr);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
}

std::string SerialPort::mac_addr() const
{
    if (transport_type() == TRANSPORT_BLUETOOTH) {
        try {
            return sp_get_port_bluetooth_address(m_SerialPort);
        } catch (const std::exception&) {
            return "Failed to retrieve";
        }
    }
    return "Unknown";
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

SerialPort SerialPorts::GetSerialPortByName(const std::string& name)
{
    sp_port* port;
    sp_return r_getport = sp_get_port_by_name(name.c_str(), &port);
    if (r_getport != SP_OK) throw std::runtime_error(
        "Failed to get serial port by name. '" + name + "'");
    return SerialPort{port};
}
/******************************************************************************/
