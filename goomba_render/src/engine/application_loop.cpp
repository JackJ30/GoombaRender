#include "application_loop.h"

#include <chrono>

namespace GoombaEngine
{
    double GetCurrentTimeInSeconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / pow(10,9);
    }
    
    ApplicationLoop::ApplicationLoop(double tickRate, double maxFrameTime)
        : m_TickRate(tickRate), m_MaxFrameTime(maxFrameTime)
    {
    }

    // Thank you GafferOnGames (https://gafferongames.com/post/fix_your_timestep/)
    void ApplicationLoop::Run()
    {
        DEBUG_ASSERT(m_RenderCallback != nullptr, "A render callback must be set for your application loop.");
        DEBUG_ASSERT(m_TickCallback != nullptr, "A tick callback must be set for your application loop.");
        
        double currentTime = GetCurrentTimeInSeconds();
        double accumulator = 0.0;
        
        m_Running = true;
        GLogInfo("Application loop started");
        while (m_Running)
        {
            double newTime = GetCurrentTimeInSeconds();
            double frameTime = newTime - currentTime;
            if ( frameTime > m_MaxFrameTime ) frameTime = m_MaxFrameTime;
            currentTime = newTime;
            
            accumulator += frameTime;
            
            while (accumulator >= m_TickRate)
            {
                m_TickCallback(m_TickRate);
                accumulator -= m_TickRate;
            }
            
            const double interpolation = accumulator / m_TickRate;
            m_RenderCallback(frameTime, interpolation);
        }
        
        GLogInfo("Application loop stopped");
    }
    
    void ApplicationLoop::Stop()
    {
        m_Running = false;
        
        GLogInfo("Application loop stopping...");
    }
} // GoombaEngine