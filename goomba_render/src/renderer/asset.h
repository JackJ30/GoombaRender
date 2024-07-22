#ifndef GOOMBARENDER_ASSET_H
#define GOOMBARENDER_ASSET_H

// TODO - "sub assets" with sub asset caching
// TODO - unloading (unloads sub assets too (eventually scenes will be an asset))
// TODO - asset rework (remove template nonesense. Asset shoud be a class, with a list of sub assets. Assets don't need to also hold their path, their load functions should return the path. Asset memory should be managed by owners. Asset loaders can handle caching)

// Asset usage
// Assets have two main functions:
// - They behave as cache-able glorified pointers to objects which are managed by this system
// - They make it easy to use the filepath and actual object as one. This way a scene doesn't need a list of model filepaths to load, and a list of models after they are loaded. It can just use a list of model assets, and load them all with the function.
// Assets should be loaded with specific load functions, or created as a sub asset in the load function of another assets.

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
        inline void SetPath(std::string path) { m_Path = path; }
    
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
