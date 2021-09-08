#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

class Timer
{
public:
    void start()
    {
        //std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<int>> tp_start(std::chrono::duration<int>(0));
        m_StartTime = std::chrono::steady_clock::now() - std::chrono::steady_clock::now();//std::chrono::system_clock::from_time_t(0);//std::chrono::steady_clock::duration::zero();
        m_bRunning = true;
    }
    
    void stop()
    {
        m_EndTime = std::chrono::steady_clock::now();
        m_bRunning = false;
    }
    
    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::steady_clock> endTime;
        
        if(m_bRunning)
        {
            endTime = std::chrono::steady_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }
        
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }
    
    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_EndTime;
    bool                                               m_bRunning = false;
};

#endif
