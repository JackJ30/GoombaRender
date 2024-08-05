#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "renderer/application.h"

int main(int argc, char *argv[])
{
    GoombaEngine::InitLogger();
    GoombaRender::RunApplication();

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
// - [x] Camera
// - [x] Refactor application
// - [x] Delta, loop, and debug
// - [x] Textures
// - [x] OpenGL abstraction objects should not make API calls in constructor (get rid of sdl)
// - [x] Window scaling and mouse
// - [x] Renderer overhaul:
//      - [x] Renderer class (load phase, draw phase)
//      - [x] abstractions should not load themselves (specific methods for loading themselves, asset handles)
//      - [x] explicitly delete oglobj
//      - [x] asset handle system
//      - [x] Model class (contains meshes, which have materials)
//      - [x] GLTF model loading (go through and load everything)
//      - [x] Scene to render instruction
// - [x] Assetloader functions, template argument assets
// - [x] Scene objects and transforms
// - [x] Rewrite
//      - [x] Application system
//      - [x] Asset system
//      - [x] OGL abstractions
// - [x] Materials and gltf material loading
// - [ ] Render pass control and material flags
// - [ ] Basic PBR lighting
// - [ ] Scene system (lights + camera ubo)
// - [ ] Time system and ms timing
// - [ ] Shadow mapping
// - [ ] Frame buffers
// - [ ] Hierarchy system
// - [ ] JSON based scene system (material override)
// - [ ] Documentation for style, includes, etc
// - [ ] Phase 1 Completed. Evaluate, rewrite goals and phases, adjust project organization, ask for feedback
//
// Wishlist
// --------
// - [ ] Ray tracing
// - [ ] Offline rendering

// DEV LOG
// -------
// 6/1/2024 - I found a conflict between stb image and the fmt included by spdlog. This is the third times fmt has caused issues for me. I had to add a definition before including spdlog in the pch that I found in some github issue. Don't know what it does.
// 6/1/2024 - I'm thinking about what to do for events/layers/windows. TheCherno's system from the Hazel series seems a little bit overkill. I will probably add some of it.
// 7/13/2024 - Just create OglObj, I think it's a pretty good solution to opengl abstractions sucking. Just used it for the texture class. First time in c++ I feel like I made a well-designed class by myself.