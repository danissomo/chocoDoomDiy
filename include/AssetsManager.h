#include "data_types.h"
#include "Patch.h"
#include "wad_loader.h"
#include "Texture.h"
#include <map>
#include <vector>

class AssetsManager{
    public:
    static AssetsManager *GetInstance();
    void Init(wad_loader *pWADLoader);

    ~AssetsManager();
    Patch *AddPatch(const std::string &patchName, WADPatchHeader &patchHeader);
    Patch *GetPatch(const std::string &patchName);

    Texture *AddTexture(WADTextureData &textureData);
    Texture *GetTexture(const std::string &sTextureName);
    
    std::string GetPName(int iPName);
    void AddPName(std::string &sPName);

    protected:
    static AssetsManager* instance;
     
    AssetsManager();

    void LoadPatch(const std::string &patchName);
    void LoadTextures();

    std::map<std::string, Patch*> m_PatchCache;
    std::map<std::string, Texture*> m_TextureCache;

    wad_loader *m_pWADLoader;

    std::vector<std::string> m_PNameLookUp;
};