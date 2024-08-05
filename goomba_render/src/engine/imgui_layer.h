#ifndef GOOMBARENDER_IMGUI_LAYER_H
#define GOOMBARENDER_IMGUI_LAYER_H

#include "window.h"
#include <imgui.h>

namespace GoombaEngine
{
    void ImGUISetup(Window& window);
    void ImGUIStartFrame();
    void ImGUIProcessEvent(SDL_Event* event);
    void ImGUIRender();
    void ImGUIShutdown();
}

#endif // IMGUI_LAYER_H