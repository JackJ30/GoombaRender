#include "renderer.h"

#include <stb/stb_image.h>
#include <sstream>
#include <fstream>

namespace GoombaRender
{
    Renderer::Renderer(GoombaEngine::GraphicsContext &context)
        : m_Context(context)
    {
    
    }
    
    Renderer::~Renderer()
    {
        for (auto& kv : m_LoadedTexture2Ds)
        {
            kv.second.Delete();
        }
        for (auto& kv : m_LoadedShaders)
        {
            kv.second.Delete();
        }
        for (auto& kv : m_LoadedModels)
        {
            kv.second.Delete();
        }
    }
    
    void Renderer::LoadScene(Scene& scene)
    {
        for (const std::string& model : scene.m_ModelsToLoad)
        {
            scene.m_Objects.push_back(LoadModel(model));
        }
    }
    
    void Renderer::AddScenePass(const Camera& camera, Scene& scene)
    {
    
    }
    
    void Renderer::Render()
    {
        for (; !m_RenderQueue.empty(); m_RenderQueue.pop()) // go through passes
        {
            RenderPass& pass = m_RenderQueue.front();
            
            for (; !pass.queue.empty(); pass.queue.pop()) // go through instructions
            {
                const RenderInstruction& instruction = pass.queue.front();
                
                GetShader(instruction.shader).Bind();
                for (int i = 0; i < instruction.textures.size(); ++i)
                {
                    GetTexture2D(instruction.textures[i]).Bind(i);
                }
                instruction.vao.Bind();
                
                m_Context.GetGlad().DrawElements(GL_TRIANGLES, instruction.vao.GetNumIndices(), GL_UNSIGNED_INT, nullptr);
            }
        }
    }
    
    Texture2DAsset Renderer::LoadTexture2D(const std::string& path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        DEBUG_ASSERT(data != nullptr, fmt::format("Could not load image at path: '{}'", path));
        DEBUG_ASSERT(channels == 3 || channels == 4, "Loaded images must have 3 or 4 channels.");
        
        Texture2D texture;
        texture.AssignContext(m_Context);
        texture.Create(data, width, height, channels);
        
        unsigned int id = m_LoadedTexture2Ds.size();
        m_LoadedTexture2Ds.insert(std::make_pair(id, texture));
        
        stbi_image_free(data);
        return { id };
    }
    
    ShaderAsset Renderer::LoadShader(const std::string& path)
    {
        std::ifstream stream(path);
        
        enum class ShaderType
        {
            NONE = 1, VERTEX = 0, FRAGMENT = 1
        };
        
        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[(int)type] << line << "\n";
            }
        }
        
        Shader shader;
        shader.AssignContext(m_Context);
        shader.Create(ss[0].str(), ss[1].str());
        
        unsigned int id = m_LoadedTexture2Ds.size();
        m_LoadedShaders.insert(std::make_pair(id, shader));
        
        return { id };
    }
    
    ModelAsset Renderer::LoadModel(const std::string& path)
    {
        return {0};
    }
    
} // GoombaRender