#include "PacketManager/Time.hpp"
#include <chrono>

using namespace std::chrono;

void Time::Init(uint32_t _msPerTick = 1)
{
    Time::Instance()->m_startTimepoint = steady_clock::now();
}
