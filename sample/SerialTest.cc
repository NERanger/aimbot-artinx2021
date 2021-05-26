#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "PacketManager/HostPacket.hpp"
#include "PacketManager/Time.hpp"

#include <iostream>

using RmAimbot::HostPacketManager;

namespace{
    float target_yaw = 0.0f;
    float target_pitch = 0.0f;
    bool if_send = false;
}

int main(int, char**){
    HostPacketManager& host_packet_manager = *HostPacketManager::Instance();
    host_packet_manager.Init("/dev/ttyUSB0", RmAimbot::SerialPortEnum::BR_921600);
    Time::Init(1);

    char key_pressed = 0;
    bool is_quitting = false;

    while (!is_quitting)
    {
        host_packet_manager.Update();

        // std::cout << "Received " << host_packet_manager.GetTestPacket().m_testData << std::endl;
        cv::namedWindow("Input window", cv::WINDOW_KEEPRATIO);
        key_pressed = cv::waitKey(10);
        switch (key_pressed)
        {
        case 'q':
            is_quitting = true;
            break;
        case 'w':
            target_yaw += 0.01f;
            // target_pitch += 0.01f;
            break;
        case 'e':
            target_yaw -= 0.01f;
            // target_pitch -= 0.01f;
            break;
        case 'r':
            target_yaw = 0.0f;
            // target_pitch = 0.0f;
            break;
        case 'f':
            target_yaw = -target_yaw;
            // target_pitch = -target_pitch;
            break;
        case 't':
            if_send = !if_send;
            break; 
        default:
            break;
        }
        if(if_send){
            host_packet_manager.GetTestPacket().cmd_yaw = target_yaw;
            host_packet_manager.GetTestPacket().cmd_pitch = target_pitch;

            std::cout << "Yaw: " << target_yaw
                  << "Pitch: " << target_pitch << std::endl; 
        }else{
            host_packet_manager.GetTestPacket().cmd_yaw = 0.0;
            host_packet_manager.GetTestPacket().cmd_pitch = 0.0;
            std::cout << "Holding" << std::endl; 
        }
        host_packet_manager.GetTestPacket().cmd_fire = 1;
        host_packet_manager.GetTestPacket().is_found = 1;
        host_packet_manager.GetTestPacket().seq++;
        host_packet_manager.GetTestPacket().SendPacket();

        unsigned int diff = host_packet_manager.GetTestPacket().seq - host_packet_manager.GetEchoPacket().seq;
        std::cout << "DIFF: " << diff << std::endl;

        key_pressed = 0;
    }

    return EXIT_SUCCESS;
}