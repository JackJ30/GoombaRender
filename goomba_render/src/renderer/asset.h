#ifndef GOOMBARENDER_ASSET_H
#define GOOMBARENDER_ASSET_H

// TODO - unloading
// TODO - non unique loading (sharing assets from same path)

#include <utility>
#include <filesystem>

namespace GoombaRender
{
    template<class T>
    std::unordered_map<unsigned int, T> loadedAssets;
    
    template<class T>
    std::unordered_map<std::filesystem::path, unsigned int> assetCache;
    
    inline unsigned int numLoaded = 0;
    
    template<class T>
    class Asset
    {
    public:
        explicit Asset(std::string path, bool unique = false) : m_Path(path), m_Unique(unique) {};
        
        T& Get() const
        {
            if (!m_Loaded) { GLogError("Asset at path '{}' must be loaded before accessing.", m_Path.string()); }
            
            return loadedAssets<T>[m_Id];
        }
        
        bool TryUseCached()
        {
            if (!m_Unique && assetCache<T>.find(m_Path) != assetCache<T>.end())
            {
                m_Loaded = true;
                m_Id = assetCache<T>[m_Path];
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
            assetCache<T>[m_Path] = m_Id;
        }
        
        inline const std::filesystem::path& GetPath() const { return m_Path; }
    
    private:
        std::filesystem::path m_Path;
        bool m_Unique = false;
        unsigned int m_Id;
        
        bool m_Loaded = false;
    };
}

#endif //GOOMBARENDER_ASSET_H
