#ifndef GOOMBARENDER_RENDERER_H
#define GOOMBARENDER_RENDERER_H

#include "renderer/camera.h"
#include "renderer/scene.h"
#include "renderer/vertex_array.h"
#include "renderer/texture.h"
#include "renderer/shader.h"
#include "renderer/model.h"
#include "renderer/asset.h"

namespace GoombaRender
{
    struct RenderInstruction // TODO - these should be references
    {
        VertexArray vao;
        Asset<Shader> shader;
        std::vector<std::pair<Asset<Texture2D>, unsigned int>> textures;
        UniformSetting uniformSetting;
    };
    
    struct RenderPass
    {
        std::queue<RenderInstruction> queue;
        // Framebuffer information
    };
    
    class Renderer
    {
    public:
        explicit Renderer(GoombaEngine::GraphicsContext& context);
        
        void AddScenePass(const Camera& camera, const Scene& scene); // TODO - include framebuffer
        void Render();
    
    private:
        GoombaEngine::GraphicsContext m_Context;
        
        std::queue<RenderPass> m_RenderQueue;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_RENDERER_H

// Pipeline
// - Load assets at start. Assets are (Textures, shaders, models, vaos). At some point assets to be loaded will be fetched from a scene
// - Passes are added that go through what should be renderered (most likely models in a scene, but can be extended to be anything), and adds render instructions to the queue
//   This step also includes setting shader all the uniforms and stuff
// - The queue is rendered by going through each render pass and instruction, bind the vao, shader, and textures, and then render