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
    struct RenderInstruction
    {
        VertexArray vao;
        ShaderAsset shader;
        std::vector<Texture2DAsset> textures;
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
        ~Renderer();
        
        void LoadScene(Scene& scene, const std::string& mainShaderPath);
        void AddScenePass(const Camera& camera, const Scene& scene); // TODO - include framebuffer
        void Render();
        
        Texture2DAsset LoadTexture2D(const std::string& path);
        ShaderAsset LoadShader(const std::string& path);
        ModelAsset LoadModel(const std::string& path);
        
        inline Texture2D& GetTexture2D(Texture2DAsset handle) { return m_LoadedTexture2Ds[handle.id]; }
        inline Shader& GetShader(ShaderAsset handle) { return m_LoadedShaders[handle.id]; }
        inline Model& GetModel(ModelAsset handle) { return m_LoadedModels[handle.id]; }
    
    private:
        GoombaEngine::GraphicsContext m_Context;
        
        std::unordered_map<unsigned int, Texture2D> m_LoadedTexture2Ds;
        std::unordered_map<unsigned int, Shader> m_LoadedShaders;
        std::unordered_map<unsigned int, Model> m_LoadedModels;
        
        std::queue<RenderPass> m_RenderQueue;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_RENDERER_H

// Pipeline
// - Load assets at start. Assets are (Textures, shaders, models, vaos). At some point assets to be loaded will be fetched from a scene
// - Passes are added that go through what should be renderered (most likely models in a scene, but can be extended to be anything), and adds render instructions to the queue
//   This step also includes setting shader all the uniforms and stuff
// - The queue is rendered by going through each render pass and instruction, bind the vao, shader, and textures, and then render