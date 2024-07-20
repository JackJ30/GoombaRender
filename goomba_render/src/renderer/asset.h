#ifndef GOOMBARENDER_ASSET_H
#define GOOMBARENDER_ASSET_H

// TODO - "sub assets"
// TODO - unloading

#include <utility>
#include <filesystem>

namespace GoombaRender
{
    template<class T>
    std::unordered_map<unsigned int, T> loadedAssets;
    
    template<class T>
    std::unordered_map<std::string, unsigned int> assetCache;
    
    inline unsigned int numLoaded = 0;
    
    template<class T>
    class Asset
    {
    public:
        Asset(bool unique = false) : m_Unique(unique) {};
        Asset(std::string path, bool unique = false) : m_Path(path), m_Unique(unique) {};
        Asset(const char* path, bool unique = false) : m_Path(path), m_Unique(unique) {};
        
        T& Get() const
        {
            if (!m_Loaded)
            {
                if (m_Path.has_value()) { GLogCritical("Asset at path '{}' must be loaded before accessing.", m_Path.value()); }
                else { GLogCritical("Asset must be loaded before accessing."); }
            }
            
            return loadedAssets<T>[m_Id];
        }
        
        bool TryLoadFromCache()
        {
            if (!m_Unique && m_Path.has_value() && assetCache<T>.find(m_Path.value()) != assetCache<T>.end())
            {
                m_Loaded = true;
                m_Id = assetCache<T>[m_Path.value()];
                
                return true;
            }
            return false;
        }
        
        void AssignLoaded(T&& loaded)
        {
            m_Loaded = true;
            m_Id = numLoaded;
            numLoaded++;
            
            loadedAssets<T>[m_Id] = loaded;
            if(m_Path.has_value()) assetCache<T>[m_Path.value()] = m_Id;
        }
        
        inline const std::optional<std::string>& GetPath() const { return m_Path; }
    
    private:
        std::optional<std::string> m_Path;
        bool m_Unique = false;
        unsigned int m_Id;
        
        bool m_Loaded = false;
    };
}

// Problems with this asset system
// - Data is in different maps for every asset type
// - LoadT functions are obligated to check for caches
// - No restrictions on asset types

#endif //GOOMBARENDER_ASSET_H
