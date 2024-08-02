#include "resource_manager.h"

#include <sstream>
#include <fstream>

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
        
        std::shared_ptr<ShaderInfo> shader = std::shared_ptr<ShaderInfo>(CreateShader(ss[0].str(), ss[1].str()));
        
        if (cache) { assetCache<ShaderInfo>[path] = shader; }
        return shader;
    }
} // GoombaRender