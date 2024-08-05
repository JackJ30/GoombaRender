#include "resource_manager.h"

#include <sstream>
#include <fstream>
#include <stb/stb_image.h>
#include <tiny_gltf.h>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GoombaRender
{
    // Setup
    template<typename T>
    std::unordered_map<std::string, std::weak_ptr<T>> assetCache;
    
    template<typename T>
    std::optional<std::shared_ptr<T>> CheckCache(const std::string& path)
    {
        auto cached = assetCache<T>.find(path);
        if (cached != assetCache<T>.end())
        {
            if (!cached->second.expired())
            {
                return cached->second.lock();
            }
            else
            {
                assetCache<T>.erase(cached);
            }
        }
        
        return std::nullopt;
    }
    
    // Loading
    std::shared_ptr<ShaderInfo> LoadShader(const std::string& path, bool cache)
    {
        if (cache)
        {
            auto cached = CheckCache<ShaderInfo>(path);
            if (cached.has_value()) { return cached.value(); }
        }
        
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
        
        std::shared_ptr<ShaderInfo> shader = std::make_shared<ShaderInfo>(CreateShader(ss[0].str(), ss[1].str()));
        
        if (cache) { assetCache<ShaderInfo>[path] = shader; }
        return shader;
    }
    
    std::shared_ptr<Texture2DInfo> LoadTexture2D(const std::string& path, GLint minFilter,
                                                 GLint magFilter, GLint wrapS, GLint wrapT, bool cache)
    {
        if (cache)
        {
            auto cached = CheckCache<Texture2DInfo>(path);
            if (cached.has_value()) { return cached.value(); }
        }
        
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        DEBUG_ASSERT(data != nullptr, fmt::format("Could not load image at path: '{}'", path));
        DEBUG_ASSERT(channels == 3 || channels == 4, "Loaded images must have 3 or 4 channels.");
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
        
        std::shared_ptr<Texture2DInfo> texture = std::make_shared<Texture2DInfo>(CreateTexture2D(data, width, height, format, GL_UNSIGNED_BYTE, minFilter, magFilter, wrapS, wrapT));
        stbi_image_free(data);
        
        if (cache) { assetCache<Texture2DInfo>[path] = texture; }
        return texture;
    }
    
    
    // TODO - support multiple uv maps
    std::shared_ptr<Model> LoadModel(const std::string& path, const std::shared_ptr<ShaderInfo>& shader, bool cache)
    {
        if (cache)
        {
            auto cached = CheckCache<Model>(path);
            if (cached.has_value()) { return cached.value(); }
        }
        std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>(shader);
        
        tinygltf::TinyGLTF loader;
        std::string error;
        std::string warn;
        
        tinygltf::Model loadedGLTF;
        bool ret = false;
        
        std::string extension = std::filesystem::path(path).extension();
        if (extension == ".gltf")
        {
            ret = loader.LoadASCIIFromFile(&loadedGLTF, &error, &warn, path);
        }
        else if (extension == ".glb")
        {
            ret = loader.LoadBinaryFromFile(&loadedGLTF, &error, &warn, path);
        }
        else { GLogError("Model must have glb or gltf file extension."); return nullptr; }
        
        if (!warn.empty()) {
            GLogError("GLTF loading warning for file '{}' :\n{}", path, warn);
        }
        if (!error.empty()) {
            GLogError("GLTF loading error for file '{}':\n{}", path, error);
            return nullptr;
        }
        
        if (!ret) { GLogError("Could not load GLTF file with path '{}'", path); return nullptr; }
        
        // --- Converting tinyGLTF to GoombaRender model ---
        // Create buffers
        std::vector<unsigned int> buffers(loadedGLTF.buffers.size());
        glad.GenBuffers(loadedGLTF.buffers.size(), buffers.data());
        for (size_t i = 0; i < loadedGLTF.buffers.size(); ++i)
        {
            glad.BindBuffer(GL_ARRAY_BUFFER, buffers[i]);
            glad.BufferData(GL_ARRAY_BUFFER, loadedGLTF.buffers[i].data.size(), loadedGLTF.buffers[i].data.data(), GL_STATIC_DRAW);
        }
        
        // Create images
        tinygltf::Sampler defaultSampler;
        defaultSampler.minFilter = GL_LINEAR;
        defaultSampler.magFilter = GL_LINEAR;
        defaultSampler.wrapS = GL_REPEAT;
        defaultSampler.wrapT = GL_REPEAT;
        
        std::vector<std::shared_ptr<Texture2DInfo>> textures;
        textures.resize(loadedGLTF.textures.size());
        for (size_t i = 0; i < loadedGLTF.textures.size(); ++i)
        {
            const tinygltf::Texture& texture = loadedGLTF.textures[i];
            DEBUG_ASSERT(texture.source >= 0, "Textures must have an image");
            const tinygltf::Image& image = loadedGLTF.images[texture.source];
            const tinygltf::Sampler& sampler = texture.sampler >= 0 ? loadedGLTF.samplers[texture.sampler] : defaultSampler;
            
            DEBUG_ASSERT(image.component == 3 || image.component == 4, "Loaded images must have 3 or 4 channels.");
            GLenum format = (image.component == 3) ? GL_RGB : GL_RGBA;
            textures[i] = std::make_shared<Texture2DInfo>(CreateTexture2D(image.image.data(), image.width, image.height, format, image.pixel_type, sampler.minFilter != -1 ? sampler.minFilter : GL_LINEAR, sampler.magFilter != -1 ? sampler.magFilter : GL_LINEAR, sampler.wrapS, sampler.wrapT));
        }
        
        // Create Materials
        std::vector<std::shared_ptr<Material>> materials;
        materials.resize(loadedGLTF.materials.size());
        for (size_t i = 0; i < loadedGLTF.materials.size(); ++i)
        {
            const tinygltf::Material& material = loadedGLTF.materials[i];
            const tinygltf::PbrMetallicRoughness& pbrMetallicRoughness = material.pbrMetallicRoughness;
            
            materials[i] = std::make_shared<Material>(shader);
            if(material.pbrMetallicRoughness.baseColorTexture.index >= 0) { materials[i]->AssignUniformTexture("u_Albedo", textures[material.pbrMetallicRoughness.baseColorTexture.index]); };
            // set uniforms and textures
            // base color (texture + factor)
            // metallic roughness (texture + factors)
            // normal (texture + scale)
            // occlusion (texture + strength)
            // emissions (texture + factor)
        }
        
        std::vector<Mesh> meshes;
        
        // Helper lambda functions
        const std::function<void(const tinygltf::Node&, glm::mat4)> traverseNode = [&](const tinygltf::Node& node, glm::mat4 parentTransform)
        {
            // Deal with node
            
            // transformation
            glm::mat4 localTransform = parentTransform;
            if (!node.matrix.empty())
            {
                localTransform *= glm::mat4(node.matrix[0], node.matrix[1],
                                            node.matrix[2], node.matrix[3], node.matrix[4],
                                            node.matrix[5], node.matrix[6], node.matrix[7],
                                            node.matrix[8], node.matrix[9], node.matrix[10],
                                            node.matrix[11], node.matrix[12], node.matrix[13],
                                            node.matrix[14], node.matrix[15]);
            }
            else
            {
                if (!node.scale.empty()) { localTransform = glm::scale(localTransform, {node.scale[0], node.scale[1], node.scale[2]} ); }
                if (!node.rotation.empty()) { localTransform *= glm::mat4_cast(glm::quat(float(node.rotation[0]), float(node.rotation[1]), float(node.rotation[2]), float(node.rotation[3]))); }
                if (!node.translation.empty()) { localTransform = glm::translate(localTransform, {node.translation[0], node.translation[1], node.translation[2]} ); }
            }
            
            if (node.mesh >= 0)
            {
                for (tinygltf::Primitive primitive : loadedGLTF.meshes[node.mesh].primitives)
                {
                    // deal with mesh (primitive)
                    Mesh mesh;
                    mesh.vao = std::make_shared<VertexArrayInfo>(CreateVertexArray(primitive.indices >= 0 ? LayoutType::Indices : LayoutType::Arrays, primitive.mode));
                    mesh.localTransform = localTransform;
                    
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
                            mesh.vao->BindAttribute(buffers[bufferIdx], attribute_indexes[i],
                                                   accessor.type, accessor.componentType,
                                                   GL_FALSE, bufferView.byteStride,
                                                   accessor.byteOffset + bufferView.byteOffset);
                        }
                    }
                    
                    // add index buffer
                    if (primitive.indices >= 0)
                    {
                        const auto accessorIdx = primitive.indices;
                        const auto &accessor = loadedGLTF.accessors[accessorIdx];
                        const auto &bufferView = loadedGLTF.bufferViews[accessor.bufferView];
                        const auto bufferIdx = bufferView.buffer;
                        
                        unsigned int count = accessor.count;
                        unsigned int type = accessor.componentType;
                        
                        IndicesSection section = {accessor.byteOffset + bufferView.byteOffset, count, type};
                        mesh.vao->SetIndexBuffer(buffers[bufferIdx], { section });
                        
                        DEBUG_ASSERT(GL_ELEMENT_ARRAY_BUFFER == bufferView.target, "It's gotta be an index buffer");
                    }
                    
                    if (primitive.material >= 0) { mesh.material = materials[primitive.material]; }
                    else { mesh.material = defaultMaterial; }
                    
                    meshes.push_back(std::move(mesh));
                }
            }
            
            // Traverse children recursively
            for (auto nodeIDX : node.children)
            {
                traverseNode(loadedGLTF.nodes[nodeIDX], localTransform);
            }
        };
        
        // Start going through all scenes and their nodes
        for (auto scene : loadedGLTF.scenes)
        {
            for (auto nodeIDX : scene.nodes)
            {
                traverseNode(loadedGLTF.nodes[nodeIDX], glm::mat4(1.0));
            }
        }
        
        std::shared_ptr<Model> model = std::make_shared<Model>(meshes, buffers);
        
        if (cache) { assetCache<Model>[path] = model; }
        return model;
    }
} // GoombaRender