# Serial communication with STM32 board

In this repo we provide a sample for [serial communication test](../sample/SerialTest.cc) and a sample for [all system integration](../sample/SysAllTest.cc).

The detection algorithm will output yaw and pitch command for gimbal. We need to use the serial port to send the command to STM32 board.

The upper-level of the serial communication system is a packet manager,  which is the same as the one used in the STM32 code. Please refer to our STM32 code manual for detailed usage and explanation.

The low-level serial port implementation leverages `boost::asio`. The implementation is from [here](https://github.com/fedetft/serial-port/tree/master/3_async).

To use the serial communication function, first include necessary header files.

```cpp
#include "PacketManager/HostPacket.hpp"
#include "PacketManager/Time.hpp"
```

Make a instance for `HostPacketManager`.

```cpp
HostPacketManager& host_packet_manager = *HostPacketManager::Instance();
```

Initialize serial port and timer. The baud rate of STM32 USART and PC serial port need to be the same.

```cpp
host_packet_manager.Init("/dev/ttyUSB0", 921600);
Time::Init(1);
```

Into the detection&communication loop.

```cpp
while(1){  // your loop condition
    
    // Acquire camera image
    // Run detection and get yaw&pitch command
    
    // Fill packet
    host_packet_manager.GetTestPacket().cmd_yaw = serial_data.yaw_cmd;
    host_packet_manager.GetTestPacket().cmd_pitch = serial_data.pitch_cmd;
    host_packet_manager.GetTestPacket().cmd_fire = serial_data.if_fire;
    host_packet_manager.GetTestPacket().is_found = serial_data.is_found;
    
    // Put packet into send buffer
    host_packet_manager.GetTestPacket().SendPacket();

	// Send & receive packets
    host_packet_manager.Update();
}
```

Here the `TestPacket` is pre-defined in `inc/PacketManager/HostPacket.hpp`, the definition is as the following:

```cpp
class TestHostPacket : public HostPacket{
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
```

A multi-thread serial sending sample is provided in [all system integration](../sample/SysAllTest.cc) sample. **Note that currently you must put serial send task in a separated thread and the sleep time of the thread should not exceed 1 millisecond.** The [all system integration](../sample/SysAllTest.cc) sample should be capable.