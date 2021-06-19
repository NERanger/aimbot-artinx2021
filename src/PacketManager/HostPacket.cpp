#include "PacketManager/HostPacket.hpp"
#include <iostream>

using namespace RmAimbot;

void HostPacket::Registration()
{
    HostPacketManager::Instance()->PacketHandlerRegistration(0, m_ProtocolId, this);
}

void HostPacket::SendPacket()
{
    HostPacketManager::Instance()->SendPacket(m_Buffer, m_DataLen + 8);
    // std::cout << (int)m_Buffer[0] << std::endl;
}

void TestHostPacket::OnPacketReceived()
{
    // SerializePacket(m_InputStream);
    // NotifyAll((void*)&m_testData);
}

void TestHostPacket::SendPacket()
{
    SerializePacket(m_OutputStream);

    HostPacket::SendPacket();
}

template<typename Stream>
void TestHostPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 12;

    SerializeHeader(stream);

    stream.SerializeFloat(cmd_yaw);
    stream.SerializeFloat(cmd_pitch);
    stream.SerializeU8(cmd_fire);
    stream.SerializeU8(is_found);
    stream.SerializeU16(seq);

    SerializeCrc16(stream);
}

void EchoPacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
    // NotifyAll((void*)&m_testData);
}

void EchoPacket::SendPacket()
{
    SerializePacket(m_OutputStream);

    HostPacket::SendPacket();
}

template<typename Stream>
void EchoPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 2;

    SerializeHeader(stream);

    stream.SerializeU16(seq);

    SerializeCrc16(stream);
}

void CameraSwitchHostPacket::OnPacketReceived()
{
    SerializePacket(m_InputStream);
}

void CameraSwitchHostPacket::SendPacket()
{
    SerializePacket(m_OutputStream);

    HostPacket::SendPacket();
}

void CameraSwitchHostPacket::SendPacket(uint8_t cameraId)
{
    m_cameraId = cameraId;

    SendPacket();
}

template<typename Stream>
void CameraSwitchHostPacket::SerializePacket(Stream &stream)
{
    m_DataLen = 1;

    SerializeHeader(stream);

    stream.SerializeU8(m_cameraId);

    SerializeCrc16(stream);
}

HostPacketManager::HostPacketManager() : PacketManager(1)
{
    m_pChannel = &m_StreamChannel;
}

void HostPacketManager::Init(std::string const &_dev_path, LibSerial::BaudRate _bandrate)
{
    // m_serialPort.SetDevPath(_dev_path);
    // m_serialPort.SetBaudRate(_bandrate);
    try{
        m_serialPort.Open(_dev_path);
    }catch(const LibSerial::OpenFailed){
        std::cerr << "[Serial] Fail to open serial port" << std::endl;
    }
    
    m_serialPort.SetBaudRate(_bandrate);
    m_serialPort.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    m_serialPort.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
    m_serialPort.SetParity(LibSerial::Parity::PARITY_NONE);
    m_serialPort.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

    // m_serialPort.setPort(_dev_path);
    // m_serialPort.setBaudrate(_bandrate);
    // m_serialPort.setBytesize(serial::eightbits);
    // m_serialPort.setParity(serial::parity_none);
    // m_serialPort.setStopbits(serial::stopbits_one);
    // m_serialPort.setFlowcontrol(serial::flowcontrol_none);

    // serial::Timeout serialTimeOut = serial::Timeout::simpleTimeout(0); 
    // m_serialPort.setTimeout(serialTimeOut);

    // // m_serialPort.OpenPort();
    // try{
    //     m_serialPort.open();
    // }catch(serial::IOException& e){
    //     std::cerr << "[Serial] Fail to open serial port" << std::endl;
    // }

    PacketManager::Init();

    m_echoPacket.Init(0x06);
    m_testPacket.Init(0x05);
    m_cameraSwitchPacket.Init(0x08);
}

void HostPacketManager::Update()
{
    LibSerial::DataBuffer buf;

    // int _readBytes = m_serialPort.Read(m_readBuffer, sizeof(m_readBuffer));
    try{
        m_serialPort.Read(buf, 10, 5);
    }catch(const LibSerial::ReadTimeout){
        std::cout << "[RE] Timeout" << std::endl;
    }

    for(size_t i = 0; i < buf.size(); ++i){
        m_readBuffer[i] = buf[i];
    }

    Enqueue(0, m_readBuffer, buf.size());
    // Enqueue(0, m_readBuffer, _readBytes);

    std::cout << "[RE] " << buf.size() << std::endl;

    PacketManager::Update();

    FlushSendBuffer();
}

bool HostPacketManager::FlushSendBufferLow()
{
    // m_serialPort.flushOutput();
    // m_serialPort.flush();
    LibSerial::DataBuffer buf(m_SendBufferLength);
    for(size_t i = 0; i < m_SendBufferLength; ++i){
        buf.push_back(m_SendBuffer[i]);
    }

    m_serialPort.Write(buf);
    // m_serialPort.Write(m_SendBuffer, m_SendBufferLength);
    // std::cout << (int)m_SendBuffer[0] << std::endl;

    return true;
}
