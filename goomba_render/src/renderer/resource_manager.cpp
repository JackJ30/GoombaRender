#include "resource_manager.h"

#include <sstream>
#include <fstream>
#include <stb/stb_image.h>

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
            if (cached.has_value()) return cached.value();
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
            if (cached.has_value()) return cached.value();
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
} // GoombaRender