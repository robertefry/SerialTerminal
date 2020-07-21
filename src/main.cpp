
#include "serial/SerialPorts.h"

#include <iostream>
#include <cstring>

static int PrintHelpMessage();
static int ListAvailableSerialPortNames();

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            return PrintHelpMessage();
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0)
            return ListAvailableSerialPortNames();
    }
}

static int PrintHelpMessage()
{
    std::cout << "A tool to interface with serial devices." << std::endl;
    std::cout << "    -h  --help        Print the help message (this message)." << std::endl;
    std::cout << "    -l  --list        List available serial devices." << std::endl;
    return 0;
}

static int ListAvailableSerialPortNames()
{
    std::cout << "Generating list of available serial ports... " << std::flush;
        SerialPorts::SerialPortList serial_ports;
    try {
        serial_ports = SerialPorts::ListAvailableSerialPorts();
    } catch (const std::runtime_error& e) {
        std::cout << std::endl << e.what() << std::endl;
        return -1;
    }
    std::cout << "Done!" << std::endl;

    for (size_t i = 0; i < serial_ports.size(); ++i)
        std::cout << "\t[" << i+1 << "] " << serial_ports[i].name() << std::endl;
    std::cout << "Found " << serial_ports.size() << " port(s) in total." << std::endl;

    return 0;
}
