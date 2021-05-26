#pragma once

#include <string>
#include <cstdint>

namespace RmAimbot
{
namespace SerialPortEnum
{
    
enum BaudRate
{
    BR_NULL,
    BR_9600,
    BR_19200,
    BR_38400,
    BR_57600,
    BR_115200,
    BR_230400,
    BR_460800,
    BR_921600
};

} // namespace SerialPortEnum

class SerialPort
{
private:
    int _serial_fd;
    std::string _dev_path;
    SerialPortEnum::BaudRate _baudrate;
public:
    SerialPort();
    SerialPort(const SerialPort&) = delete;
    SerialPort(SerialPort&&) = delete;
    ~SerialPort();

    bool OpenPort();
    bool ClosePort();

    int Read(uint8_t* buffer, size_t buffer_size);
    int Write(uint8_t* buffer, size_t buffer_size);
    int Flush();

    bool IsOpened() const{ return _serial_fd != -1; }
    void SetDevPath(std::string const &devPath) { _dev_path = devPath; }
    std::string GetDevPath()const { return _dev_path; }
    void SetBaudRate(SerialPortEnum::BaudRate baudrate) { _baudrate = baudrate; }
    SerialPortEnum::BaudRate GetBaudRate()const { return _baudrate; }
};

} // namespace RmAimbot

