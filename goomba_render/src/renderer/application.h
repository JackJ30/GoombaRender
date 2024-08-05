#ifndef GOOMBARENDER_APPLICATION_H
#define GOOMBARENDER_APPLICATION_H

#include "engine/window.h"

namespace GoombaRender
{
    void RunApplication();
    
    void Tick(double delta);
    void Render(double delta, double interpolation);
    void OnEvent(SDL_Event& event);
} // GoombaRender

#endif //GOOMBARENDER_APPLICATION_H
