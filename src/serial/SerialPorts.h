
#pragma once

#include <vector>
#include <string>

class sp_port;

/******************************************************************************/
/* SerialPort *****************************************************************/
class SerialPort final
{
public:
    explicit SerialPort(sp_port* port);
    ~SerialPort();

    SerialPort(const SerialPort& other);
    SerialPort(SerialPort&& other);
    SerialPort& operator=(const SerialPort& other);
    SerialPort& operator=(SerialPort&& other);

    enum TransportType
    {
        TRANSPORT_NATIVE,
        TRANSPORT_USB,
        TRANSPORT_BLUETOOTH,
        TRANSPORT_UNKNOWN,
    };
    TransportType transport_type() const;

    std::string name() const;
    std::string description() const;
    std::string transport() const;
    std::string manufacturer() const;
    std::string product() const;
    std::string serial() const;
    std::string vendor_id() const;
    std::string product_id() const;
    std::string usb_bus() const;
    std::string usb_addr() const;
    std::string mac_addr() const;

private:
    sp_port* m_SerialPort;
};
/******************************************************************************/

/******************************************************************************/
/* Serial Ports ***************************************************************/
namespace SerialPorts
{
    using SerialPortList = std::vector<SerialPort>;
    extern SerialPortList ListAvailableSerialPorts();

    extern SerialPort GetSerialPortByName(const std::string& name);
}
/******************************************************************************/
