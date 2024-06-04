#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "engine/window/sdl_window.h"

namespace GoombaEngine
{
    void ImGUISetup(SDLWindow& window);
    void ImGUIStartFrame();
    void ImGUIProcessEvent(SDL_Event* event);
    void ImGUIRender();
    void ImGUIShutdown();
}

#endif // IMGUI_LAYER_H