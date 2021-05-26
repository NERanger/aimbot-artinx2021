#pragma once

#include <cstdint>
#include <chrono>

class Time
{
private:
    Time() : m_CurrentTick(0),
             m_MsPerTick(1)
    {
    }

    uint32_t m_CurrentTick;
    uint32_t m_MsPerTick;
    std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
public:
    static Time* Instance()
    {
        static Time instance;
        return &instance;
    }

    static void Init(uint32_t _msPerTick);
    static uint32_t GetMsPerTick(){ return Instance()->m_MsPerTick; }
    static uint32_t GetTick()
    {
        return (uint32_t)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - Instance()->m_startTimepoint).count());
    }
    static void Tick(){ ++Instance()->m_CurrentTick; }
};
