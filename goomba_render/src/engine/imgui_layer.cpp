#include "imgui_layer.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

namespace GoombaEngine
{
    void ImGUISetup(SDLWindow& window)
    {
        GLogInfo("Initializing ImGUI...");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForOpenGL(window.GetHandle(), window.GetSDLContext());
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    void ImGUIStartFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void ImGUIProcessEvent(SDL_Event *event)
    {
        ImGui_ImplSDL3_ProcessEvent(event);
    }

    void ImGUIRender()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGUIShutdown()
    {
        GLogInfo("Shutting down ImGUI...");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}