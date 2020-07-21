
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

    std::string name() const;

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
}
/******************************************************************************/
