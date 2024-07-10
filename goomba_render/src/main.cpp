#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "engine/engine.h"
#include "renderer/renderer_application.h"

int main(int argc, char *argv[])
{
    GoombaRender::RendererApplication game;
    GoombaEngine::RunApplication(game);

    return 0;
}

// TODO LIST
// ---------
// - [x] Logger
// - [x] Add event system
// - [x] Move opengl context stuff to a subclass of window? Or maybe it should be in a renderer class (update: I meod it to glad_context_creator and used another function pointer in glfwwindow)
// - [x] Escape to exit window
// - [x] Maybe create renderer_window? Decide on where the renderer will exist, and how it will interact with multiple windows.
// - [x] Hardcoded ImGUI
// - [x] OpenGL abstractions
// - [x] SDL input
// - [ ] Camera
// - [ ] Textures
// - [ ] Allow OpenGL abstraction objects to be bound to context later
// - [ ] Hardcoded render pipeline
// - [ ] Model loading
// - [ ] Code based scene system
// - [ ] Generalized pipeline
// - [ ] Material system
// - [ ] Forward rendering lighting
// - [ ] Basic render loop
// - [ ] Basic delta time
//
// Wishlist
// --------
// - [ ] Ray tracing
// - [ ] Offline rendering

// DEV LOG
// -------
// 6/1/2024 - I found a conflict between stb image and the fmt included by spdlog. This is the third times fmt has caused issues for me. I had to add a definition before including spdlog in the pch that I found in some github issue. Don't know what it does.
// 6/1/2024 - I'm thinking about what to do for events/layers/windows. TheCherno's system from the Hazel series seems a little bit overkill. I will probably add some of it it.