#include "data_types.h"
#include "Patch.h"
#include "wad_loader.h"
#include <map>
class AssetsManager{
    public:
    static AssetsManager *GetInstance();
    void Init(wad_loader *pWADLoader);

    ~AssetsManager();
    Patch *AddPatch(const std::string &patchName, WADPatchHeader &patchHeader);
    Patch *GetPatch(const std::string &patchName);



    protected:
    static AssetsManager* instance;
     
    AssetsManager();

    void LoadPatch(const std::string &patchName);

    std::map<std::string, Patch*> m_PatchCache;

    wad_loader *m_pWADLoader;
};