
#include "serial/SerialPorts.h"

#include <iostream>
#include <cstring>

static int PrintHelpMessage();
static int ListAvailableSerialPortNames();
static int PrintSerialPortInformation(const char* name);

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            return PrintHelpMessage();
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0)
            return ListAvailableSerialPortNames();
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0)
            return PrintSerialPortInformation(argv[i+1]);
    }
}

static int PrintHelpMessage()
{
    std::cout << "A tool to interface with serial devices." << std::endl;
    std::cout << "    -h  --help              Print the help message (this message)." << std::endl;
    std::cout << "    -l  --list              List available serial devices." << std::endl;
    std::cout << "    -i  --info  <device>    Show information about the serial device <device>." << std::endl;
    return 0;
}

static int ListAvailableSerialPortNames()
{
    std::cout << "Generating list of available serial ports... " << std::flush;
    SerialPorts::SerialPortList serial_ports;
    try {
        serial_ports = SerialPorts::ListAvailableSerialPorts();
    } catch (const std::exception& e) {
        std::cout << std::endl << e.what() << std::endl;
        return -1;
    }
    std::cout << "Done!" << std::endl;

    for (size_t i = 0; i < serial_ports.size(); ++i)
        std::cout << "\t[" << i+1 << "] " << serial_ports[i].name() << std::endl;
    std::cout << "Found " << serial_ports.size() << " port(s) in total." << std::endl;

    return 0;
}

static int PrintSerialPortInformation(const char* name)
{
    try {
        SerialPort port = SerialPorts::GetSerialPortByName(name);
        std::cout << "Port name:     " << port.name() << std::endl;
        std::cout << "Description:   " << port.description() << std::endl;
        std::cout << "Transport:     " << port.transport() << std::endl;
        if (port.transport_type() == SerialPort::TRANSPORT_USB) {
            std::cout << "Manufacturer:  " << port.manufacturer() << std::endl;
            std::cout << "Product:       " << port.product() << std::endl;
            std::cout << "Serial Number: " << port.serial() << std::endl;
            std::cout << "PID:           " << port.product_id() << std::endl;
            std::cout << "VID:           " << port.vendor_id() << std::endl;
            std::cout << "USB bus:       " << port.usb_bus() << std::endl;
            std::cout << "USB address:   " << port.usb_addr() << std::endl;
        }
        if (port.transport_type() == SerialPort::TRANSPORT_BLUETOOTH) {
            std::cout << "MAC address:   " << port.mac_addr() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
