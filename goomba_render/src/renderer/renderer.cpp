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
                
                m_Context.GetGlad().DrawElements(GL_TRIANGLES, instruction.vao.GetNumVertices(), GL_UNSIGNED_INT, nullptr);
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
        tinygltf::TinyGLTF loader;
        std::string error;
        std::string warn;
        
        tinygltf::Model loadedGLTF;
        bool ret = loader.LoadASCIIFromFile(&loadedGLTF, &error, &warn, path);
        
        if (!warn.empty()) {
            GLogError("GLTF loading warning for file '{}' :\n{}", path, warn);
        }
        if (!error.empty()) {
            GLogError("GLTF loading error for file '{}' :\n{}", path, error);
        }
        
        if (!ret) { GLogCritical("Could not load GLTF file with path '{}'", path); }
        
        Model model;
        model.AssignContext(m_Context);
        model.Create();
        
        // --- Converting tinyGLTF to GoombaRender model ---
        
        // Create buffers
        model.GetBuffers().reserve(loadedGLTF.buffers.size());
        m_Context.GetGlad().GenBuffers(loadedGLTF.buffers.size(), model.GetBuffers().data());
        for (size_t i = 0; i < loadedGLTF.buffers.size(); ++i)
        {
            m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, model.GetBuffers()[i]);
            m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, loadedGLTF.buffers[i].data.size(), loadedGLTF.buffers[i].data.data(), GL_STATIC_DRAW);
        }
        
        // Helper lambda functions
        const std::function<void(const tinygltf::Node&)> traverseNode = [&](const tinygltf::Node& node)
        {
            // Deal with node
            // TODO - transform
            for (tinygltf::Primitive primitive : loadedGLTF.meshes[node.mesh].primitives)
            {
                // deal with mesh (primitive)
                Mesh mesh;
                mesh.vao.AssignContext(m_Context);
                mesh.vao.Create(primitive.indices >= 0 ? DrawType::Indices : DrawType::Arrays);
                
                // go through attributes
                std::string attributes[3] = {"POSITION", "NORMAL", "TEXCOORD_0"};
                unsigned int attribute_indexes[3] = {0, 1, 2};
                
                for (int i = 0; i < 3; ++i)
                {
                    if (primitive.attributes.count(attributes[i]) > 0)
                    {
                        const auto accessorIdx = primitive.attributes[attributes[i]];
                        const auto &accessor = loadedGLTF.accessors[accessorIdx];
                        const auto &bufferView = loadedGLTF.bufferViews[accessor.bufferView];
                        const auto bufferIdx = bufferView.buffer;
                        
                        DEBUG_ASSERT(GL_ARRAY_BUFFER == bufferView.target, "It's gotta be an array buffer");
                        mesh.vao.BindAttribute(model.GetBuffers()[bufferIdx], attribute_indexes[i], accessor.type, accessor.componentType,
                                          GL_FALSE, GLsizei(bufferView.byteStride),
                                          accessor.byteOffset + bufferView.byteOffset);
                    }
                }
                
                if (primitive.indices >= 0)
                {
                    const auto accessorIdx = primitive.indices;
                    const auto &accessor = loadedGLTF.accessors[accessorIdx];
                    const auto &bufferView = loadedGLTF.bufferViews[accessor.bufferView];
                    const auto bufferIdx = bufferView.buffer;
                    
                    unsigned int count = accessor.count;
                    
                    IndicesSection section = {accessor.byteOffset + bufferView.byteOffset, count};
                    mesh.vao.SetIndexBuffer(model.GetBuffers()[bufferIdx], { section });
                    
                    DEBUG_ASSERT(GL_ELEMENT_ARRAY_BUFFER == bufferView.target, "It's gotta be an index buffer");
                }
                
                model.AddMesh(mesh);
            }
            
            // Traverse children recursively
            for (auto nodeIDX : node.children)
            {
                traverseNode(loadedGLTF.nodes[nodeIDX]);
            }
        };
        
        // Start going through all scenes and their nodes
        for (auto scene : loadedGLTF.scenes)
        {
            for (auto nodeIDX : scene.nodes)
            {
                traverseNode(loadedGLTF.nodes[nodeIDX]);
            }
        }
        
        // the plan
        // - one mesh per primitive
        // - go through materials, load textures
        // - go through meshes and primitives, create their vao's and buffers
        // - go through
        
        // todo
        // - [x] load in meshes from all nodes and their buffers
        // - [ ] transforms
        // - [ ] materials
        
        unsigned int id = m_LoadedModels.size();
        m_LoadedModels.insert(std::make_pair(id, model));
        
        return { id };
    }
    
} // GoombaRender