#ifndef GOOMBARENDER_APPLICATION_LOOP_H
#define GOOMBARENDER_APPLICATION_LOOP_H

namespace GoombaEngine
{
    class ApplicationLoop
    {
    public:
        ApplicationLoop(double tickRate = 0.00694, double maxFrameTime = 0.25);
        
        void Run();
        void Stop();
        
        inline void RegisterTickCallback(std::function<void(double delta)> tickCallback) { m_TickCallback = tickCallback; }
        inline void RegisterRenderCallback(std::function<void(double delta, double interpolation)> renderCallback) { m_RenderCallback = renderCallback; }
    
        double GetFrameTime() const;
        
        inline double GetTickRate() const { return m_TickRate; }
        inline double GetMaxFrameTime() const { return m_MaxFrameTime; }
        inline double IsRunning() const { return m_Running; }
        
    private:
        bool m_Running = false;
        double m_TickRate;
        double m_MaxFrameTime;
        
        std::function<void(double delta)> m_TickCallback;
        std::function<void(double delta, double interpolation)> m_RenderCallback;
    };
    
} // GoombaEngine

#endif //GOOMBARENDER_APPLICATION_LOOP_H
