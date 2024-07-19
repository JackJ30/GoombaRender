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
    
    inline unsigned int numLoaded = 0;
    
    template<class T>
    class Asset
    {
    public:
        explicit Asset(std::string path) : m_Path(path) {};
        
        T& Get() const
        {
            if (!m_Loaded) { GLogError("Asset at path '{}' must be loaded before accessing.", m_Path.string()); }
            
            return loadedAssets<T>[m_Id];
        }
        
        void AssignLoaded(T&& loaded)
        {
            m_Loaded = true;
            m_Id = numLoaded;
            numLoaded++;
            loadedAssets<T>[m_Id] = loaded;
        }
        
        inline const std::filesystem::path& GetPath() const { return m_Path; }
    
    private:
        std::filesystem::path m_Path;
        bool m_Loaded = false;
        unsigned int m_Id;
    };
}

#endif //GOOMBARENDER_ASSET_H
