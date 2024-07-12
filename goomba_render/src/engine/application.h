#ifndef GOOMBARENDER_APPLICATION_H
#define GOOMBARENDER_APPLICATION_H


namespace GoombaEngine
{
    class Application
    {
    public:
        Application() = default;
        virtual ~Application() = default;

        virtual void Run() = 0;
    };
}

#endif // APPLICATION_H