#pragma once

#include "PacketManager/Packet.hpp"
// #include "SerialPort/SerialPort.hpp"
// #include <libserial/SerialPort.h>
// #include <libserial/SerialStream.h>
#include "AsyncSerial/BufferedAsyncSerial.h"

#include <string>

namespace RmAimbot
{

class HostPacket : public Packet
{
public:
    HostPacket(){}

    virtual void Init(uint8_t _id)
    {
        Packet::Init(_id);

        Registration();
    }

    virtual void Registration();
    virtual void SendPacket();
};

class TestHostPacket : public HostPacket
{
public:
	TestHostPacket(){}
    float cmd_yaw;
    float cmd_pitch;
    uint8_t cmd_fire;
    uint8_t is_found;
    uint16_t seq;

    virtual void Init(uint8_t _id)
    {
        HostPacket::Init(_id);

        cmd_yaw = 0.0f;
        cmd_pitch = 0.0f;
        cmd_fire = 0;
        is_found = 0;
        seq = 0;
    }

    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class EchoPacket : public HostPacket
{
public:
	EchoPacket(){}
    uint16_t seq;

    virtual void Init(uint8_t _id)
    {
        HostPacket::Init(_id);

        seq = 0;
    }

    virtual void OnPacketReceived();
    virtual void SendPacket();
    template<typename Stream> void SerializePacket(Stream &stream);
};

class CameraSwitchHostPacket : public HostPacket
{
public:
	CameraSwitchHostPacket(){}
    uint8_t m_cameraId;

    virtual void Init(uint8_t _id)
    {
        HostPacket::Init(_id);

        m_cameraId = 0x00;
    }

    virtual void OnPacketReceived();
    virtual void SendPacket();
    void SendPacket(uint8_t cameraId);
    template<typename Stream> void SerializePacket(Stream &stream);
};

class HostPacketManager : public PacketManager
{
private:
    StreamChannel m_StreamChannel;

    TestHostPacket m_testPacket;
    EchoPacket m_echoPacket;
    CameraSwitchHostPacket m_cameraSwitchPacket;
    // SerialPort m_serialPort;
    BufferedAsyncSerial::Ptr m_serialPort;
    uint8_t m_readBuffer[2048];

protected:
    virtual bool FlushSendBufferLow();

public:
    HostPacketManager();

    EchoPacket& GetEchoPacket()
    {
        return m_echoPacket;
    }

    TestHostPacket& GetTestPacket()
    {
        return m_testPacket;
    }

    CameraSwitchHostPacket& GetCameraSwitchPacket()
    {
        return m_cameraSwitchPacket;
    }

    BufferedAsyncSerial::Ptr& GetSerialPort()
    {
        return m_serialPort;
    }

    // virtual void Init(std::string const &_dev_path, SerialPortEnum::BaudRate _bandrate);
    virtual void Init(std::string const &_dev_path, unsigned int _bandrate);
    virtual void Update();

    static HostPacketManager* Instance()
    {
        static HostPacketManager instance;
        return &instance;
    }
};

};
