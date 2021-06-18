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

    int max_diff = -1;
}

int main(int, char**){
    HostPacketManager& host_packet_manager = *HostPacketManager::Instance();
    host_packet_manager.Init("/dev/ttyUSB0", 115200);
    Time::Init(1);

    char key_pressed = 0;
    bool is_quitting = false;

    while (!is_quitting)
    {
        host_packet_manager.Update();

        // std::cout << "Received " << host_packet_manager.GetTestPacket().m_testData << std::endl;
        cv::namedWindow("Input window", cv::WINDOW_KEEPRATIO);
        key_pressed = cv::waitKey(1);
        switch (key_pressed)
        {
        case 'q':
            is_quitting = true;
            break;
        case 'w':
            // target_yaw += 0.01f;
            target_pitch += 0.01f;
            break;
        case 'e':
            // target_yaw -= 0.01f;
            target_pitch -= 0.01f;
            break;
        case 'r':
            // target_yaw = 0.0f;
            target_pitch = 0.0f;
            break;
        case 'f':
            // target_yaw = -target_yaw;
            target_pitch = -target_pitch;
            break;
        case 't':
            if_send = !if_send;
            break;
        case 'z':
            host_packet_manager.GetTestPacket().seq = 440.0f * (float)pow(2, (float)1/(float)12);
            break;
        case 'x':
            host_packet_manager.GetTestPacket().seq = 440.0f * (float)pow(2, (float)2/(float)12);
            break;
        case 'c':
            host_packet_manager.GetTestPacket().seq = 440.0f * (float)pow(2, (float)3/(float)12);
            break; 
        case 'v':
            host_packet_manager.GetTestPacket().seq = 440.0f * (float)pow(2, (float)4/(float)12);
            break; 
        case 'b':
            host_packet_manager.GetTestPacket().seq = 440.0f * (float)pow(2, (float)5/(float)12);
            break;
        case 'n':
            host_packet_manager.GetTestPacket().seq = 440.0f * (float)pow(2, (float)6/(float)12);
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
        // host_packet_manager.GetTestPacket().seq++;
        host_packet_manager.GetTestPacket().SendPacket();

        int diff = host_packet_manager.GetTestPacket().seq - host_packet_manager.GetEchoPacket().seq;

        if(diff > max_diff){
            max_diff = diff;
        }

        std::cout << "[DEBUG INFO]" << std::endl
                  << "-- Send seq: " << host_packet_manager.GetTestPacket().seq << std::endl
                  << "-- Recv seq: " << host_packet_manager.GetEchoPacket().seq << std::endl
                  << "-- DIFF: " << diff << std::endl
                  << "-- MAX DIFF: " << max_diff << std::endl;

        key_pressed = 0;
    }

    return EXIT_SUCCESS;
}