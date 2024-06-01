namespace GoombaRender
{
    class Application
    {
    public:
        Application() = default;
        virtual ~Application() = default;

        void Start();

        inline void Stop()
        {
            m_Running = false;
        }

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
    
    private:
        bool m_Running = false;
    };

    //Application* GetApplication();
}