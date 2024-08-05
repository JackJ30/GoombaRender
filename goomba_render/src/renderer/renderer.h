#ifndef GOOMBARENDER_RENDERER_H
#define GOOMBARENDER_RENDERER_H

#include "renderer/vertex_array.h"
#include "renderer/texture.h"
#include "renderer/shader.h"
#include "renderer/camera.h"
#include "scene.h"
#include "material.h"

namespace GoombaRender
{
    struct RenderInstruction
    {
        RenderInstruction(const VertexArrayInfo& vao, const Material& material)
                : vao(vao), material(material) {}
        
        const VertexArrayInfo& vao;
        const Material& material;
    };
    
    struct RenderPass
    {
        std::queue<RenderInstruction> queue;
        // Framebuffer information
    };
    
    class Renderer
    {
    public:
        void Initialize();
        void AddScenePass(const Camera& camera, const Scene& scene); // TODO - include framebuffer
        void Render();
    
    private:
        std::queue<RenderPass> m_RenderQueue;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_RENDERER_H

// Pipeline
// - Load assets at start. Assets are (Textures, shaders, models, vaos). At some point assets to be loaded will be fetched from a scene
// - Passes are added that go through what should be renderered (most likely models in a scene, but can be extended to be anything), and adds render instructions to the queue
//   This step also includes setting shader all the uniforms and stuff
// - The queue is rendered by going through each render pass and instruction, bind the vao, shader, and textures, and then render