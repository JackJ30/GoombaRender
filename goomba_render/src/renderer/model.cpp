#include "model.h"

#include <glm/gtc/quaternion.hpp>

namespace GoombaRender
{
    void Model::Create()
    {
        RequireContext();
        m_Created = true;
    }
    
    void Model::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Model must be created before deleting.");
        
        for (auto& mesh : m_Meshes)
        {
            mesh.vao.Delete();
        }
        m_Context.GetGlad().DeleteBuffers(m_Buffers.size(), m_Buffers.data());
    }
    
    void Model::AddMesh(const Mesh& mesh)
    {
        m_Meshes.push_back(mesh); // TODO - move this
    }
    
    // Loading
    
    /*struct ModelLoaderSettings
    {
        Asset<Shader> pbrShader;
        Material defaultMaterial;
        
        bool initialized = false;
    } modelLoaderSettings;
    
    void InitializeModelLoader(Asset<Shader> pbrShader, GoombaEngine::GraphicsContext& context)
    {
        modelLoaderSettings.pbrShader = pbrShader;
        //modelLoaderSettings.defaultMaterial.Create(pbrShader);
        
        modelLoaderSettings.initialized = true;
    }*/ // TODO - use a tailored pbr shader with only the features (textures) that the material has
    
    void LoadModel(Asset<Model> &asset, GoombaEngine::GraphicsContext &context)
    {
        //DEBUG_ASSERT(modelLoaderSettings.initialized, "Model loader must be initialized before loading a model.");
        if (asset.TryLoadFromCache()) { return; }
        if (!asset.GetPath().has_value()) { GLogError("Can not load model with no path."); return; }
        
        tinygltf::TinyGLTF loader;
        std::string error;
        std::string warn;
        
        tinygltf::Model loadedGLTF;
        bool ret = false;
        
        std::string extension = std::filesystem::path(asset.GetPath().value()).extension();
        if (extension == ".gltf")
        {
            ret = loader.LoadASCIIFromFile(&loadedGLTF, &error, &warn, asset.GetPath().value());
        }
        else if (extension == ".glb")
        {
            ret = loader.LoadBinaryFromFile(&loadedGLTF, &error, &warn, asset.GetPath().value());
        }
        else { GLogError("Model must have glb or gltf file extension."); return; }
        
        if (!warn.empty()) {
            GLogError("GLTF loading warning for file '{}' :\n{}", asset.GetPath().value(), warn);
        }
        if (!error.empty()) {
            GLogError("GLTF loading error for file '{}' :\n{}", asset.GetPath().value(), error);
            return;
        }
        
        if (!ret) { GLogError("Could not load GLTF file with path '{}'", asset.GetPath().value()); return; }
        
        Model model;
        model.AssignContext(context);
        model.Create();
        
        // --- Converting tinyGLTF to GoombaRender model ---
        
        // Create buffers
        model.GetBuffers().reserve(loadedGLTF.buffers.size());
        context.GetGlad().GenBuffers(loadedGLTF.buffers.size(), model.GetBuffers().data());
        for (size_t i = 0; i < loadedGLTF.buffers.size(); ++i)
        {
            context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, model.GetBuffers()[i]);
            context.GetGlad().BufferData(GL_ARRAY_BUFFER, loadedGLTF.buffers[i].data.size(), loadedGLTF.buffers[i].data.data(), GL_STATIC_DRAW);
        }
        
        // Create images
        tinygltf::Sampler defaultSampler;
        defaultSampler.minFilter = GL_LINEAR;
        defaultSampler.magFilter = GL_LINEAR;
        defaultSampler.wrapS = GL_REPEAT;
        defaultSampler.wrapT = GL_REPEAT;
        
        std::vector<Asset<Texture2D>> textures;
        textures.resize(loadedGLTF.textures.size());
        for (size_t i = 0; i < loadedGLTF.textures.size(); ++i)
        {
            const tinygltf::Texture& texture = loadedGLTF.textures[i];
            DEBUG_ASSERT(texture.source >= 0, "Textures must have an image");
            const tinygltf::Image& image = loadedGLTF.images[texture.source];
            const tinygltf::Sampler& sampler = texture.sampler >= 0 ? loadedGLTF.samplers[texture.sampler] : defaultSampler;
            
            Texture2D oglTexture;
            oglTexture.AssignContext(context);
            
            TextureFilterType min, mag; // TODO - support mipmapping
            if (sampler.minFilter >= 0 && (sampler.minFilter == GL_NEAREST || sampler.minFilter == GL_NEAREST_MIPMAP_LINEAR || sampler.minFilter == GL_NEAREST_MIPMAP_NEAREST)) { min = TextureFilterType::Nearest; }
            else { min = TextureFilterType::Linear; }
            if (sampler.magFilter >= 0 && (sampler.magFilter == GL_NEAREST || sampler.magFilter == GL_NEAREST_MIPMAP_LINEAR || sampler.magFilter == GL_NEAREST_MIPMAP_NEAREST)) { mag = TextureFilterType::Nearest; }
            else { mag = TextureFilterType::Linear; }
            oglTexture.SetFiltering(min, mag);
            
            DEBUG_ASSERT(image.component == 3 || image.component == 4, "Loaded images must have 3 or 4 channels.");
            GLenum format = (image.component == 3) ? GL_RGB : GL_RGBA;
            
            oglTexture.SetWrapping(sampler.wrapS, sampler.wrapT);
            oglTexture.Create(image.image.data(), image.width, image.height, format, image.pixel_type);
            // TODO - support mipmapping
            
            textures[i].AssignLoaded(std::move(oglTexture));
        }
        
        /*
        // Create Materials
        std::vector<Asset<Material>> materials;
        materials.resize(loadedGLTF.materials.size());
        for (size_t i = 0; i < loadedGLTF.materials.size(); ++i)
        {
            // Get default texture
            const tinygltf::Material& material = loadedGLTF.materials[i];
            const tinygltf::PbrMetallicRoughness& pbrMetallicRoughness = material.pbrMetallicRoughness;
            
            Material oglMaterial;
            oglMaterial.AssignContext(context);
            oglMaterial.Create(modelLoaderSettings.pbrShader);
            
            // set uniforms and textures
            // base color (texture + factor)
            // metallic roughness (texture + factors)
            // normal (texture + scale)
            // occlusion (texture + strength)
            // emissions (texture + factor)
            
            materials[i].AssignLoaded(std::move(oglMaterial));
        }*/
        
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
                    mesh.vao.AssignContext(context);
                    mesh.vao.Create(primitive.indices >= 0 ? DrawType::Indices : DrawType::Arrays, primitive.mode);
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
                            mesh.vao.BindAttribute(model.GetBuffers()[bufferIdx], attribute_indexes[i],
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
                        mesh.vao.SetIndexBuffer(model.GetBuffers()[bufferIdx], { section });
                        
                        DEBUG_ASSERT(GL_ELEMENT_ARRAY_BUFFER == bufferView.target, "It's gotta be an index buffer");
                    }
                    
                    // add materials (or default material)
                    
                    model.AddMesh(mesh);
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
        
        asset.AssignLoaded(std::move(model));
    }
} // GoombaRender