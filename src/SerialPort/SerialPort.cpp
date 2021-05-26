#include "SerialPort/SerialPort.hpp"
#include "PacketManager/Time.hpp"

// Linux file control
#include <fcntl.h>
// POSIX terminal control definition
#include <termios.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>

using namespace RmAimbot;
using namespace SerialPortEnum;
namespace{
    uint32_t last_send_time = -1.0;
}

SerialPort::SerialPort():_baudrate(BR_NULL),
                         _dev_path("")
{
    ;
}

SerialPort::~SerialPort()
{
    if(!IsOpened())
    {
        ClosePort();
    }
}

bool SerialPort::OpenPort()
{
    if(BR_NULL == _baudrate)
    {
        std::cerr << "[SerialPort]Baudrate is not set!" << std::endl;
        return false;
    }

    if(_dev_path.compare("") == 0)
    {
        std::cerr << "[SerialPort]Device path is not set!" << std::endl;
        return false;
    }

    // _serial_fd =  open(_dev_path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    _serial_fd =  open(_dev_path.c_str(), O_RDWR | O_NOCTTY);
    if(!IsOpened())
    {
        std::cerr << "[SerialPort]Error opening serial port. Errno: " << errno << std::endl;
        return false;
    }

    fcntl(_serial_fd, F_SETFL, 0);

    termios serial_port_options;
    // Read in existion settings.
    if(tcgetattr(_serial_fd, &serial_port_options) != 0)
    {
        std::cerr << "[SerialPort]Error getting config. Errno: " << errno << std::endl;
        close(_serial_fd);
        return false;
    }
    
    // Read serial port as raw data.
    cfmakeraw(&serial_port_options);
    
    int baudrate_raw = B0;
    switch (_baudrate)
    {
    case BR_9600:
        baudrate_raw = B9600;
        break;
    case BR_19200:
        baudrate_raw = B19200;
        break;
    case BR_38400:
        baudrate_raw = B38400;
        break;
    case BR_57600:
        baudrate_raw = B57600;
        break;
    case BR_115200:
        baudrate_raw = B115200;
        break;
    case BR_230400:
        baudrate_raw = B230400;
        break;
    case BR_460800:
        baudrate_raw = B460800;
        break;
    case BR_921600:
        baudrate_raw = B921600;
        break;
    default:
        break;
    }

    cfsetispeed(&serial_port_options, baudrate_raw);
    cfsetospeed(&serial_port_options, baudrate_raw);

    serial_port_options.c_cc[VTIME] = 0;
    serial_port_options.c_cc[VMIN] = 0;

    if(tcsetattr(_serial_fd, TCSANOW, &serial_port_options) != 0)
    {
        std::cerr << "[SerialPort]Error setting config. Errno: " 
                  << errno << " " << std::endl;
        close(_serial_fd);
        return false;
    }

    tcflush(_serial_fd, TCIOFLUSH);

    return true;
}

bool SerialPort::ClosePort()
{
    close(_serial_fd);
    return true;
}

int SerialPort::Flush()
{
    return tcflush(_serial_fd, TCIOFLUSH);
}

int SerialPort::Read(uint8_t* buffer, size_t buffer_size)
{
    int read_byte = read(_serial_fd, buffer, buffer_size);
    tcflush(_serial_fd, TCIFLUSH);
    return read_byte;
}

int SerialPort::Write(uint8_t* buffer, size_t buffer_size)
{
    // return write(_serial_fd, buffer, buffer_size);
    if(last_send_time < 0.0f){
        last_send_time = Time::GetTick();
    }else{
        std::cout << "Time span(ms): " << Time::GetTick() - last_send_time;
        last_send_time = Time::GetTick();
    }
    int len = 0;
    Flush();
    len = write(_serial_fd, buffer, buffer_size);
    // tcdrain(_serial_fd);
    if (len == buffer_size ){
        // printf("send data is %s\n",send_buf);
        return len;
    }
    else{
        std::cout << "Send data: "
                  << len << std::endl;
        tcflush(_serial_fd,TCOFLUSH);
        return len;
    }
}
