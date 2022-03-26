#include "AssetsManager.h"

AssetsManager* AssetsManager::instance =nullptr;


AssetsManager::AssetsManager(){}


AssetsManager::~AssetsManager(){
    for (auto patch = m_PatchCache.begin(); patch != m_PatchCache.end(); patch++)
        delete patch->second;
}


void AssetsManager::Init(wad_loader *pWADLoader){
    m_pWADLoader = pWADLoader;
}


Patch* AssetsManager::GetPatch(const std::string &patchName){
    if(m_PatchCache.count(patchName) >0)
        return m_PatchCache[patchName];
    LoadPatch(patchName);
    return m_PatchCache[patchName];
}


Patch* AssetsManager::AddPatch(const std::string &patchName, WADPatchHeader &patchHeader){
    Patch* patch = new Patch(patchName); 
    m_PatchCache[patchName] = patch;
    patch->InitPatch(patchHeader);
    return patch;
}


void AssetsManager::LoadPatch(const std::string &patchName){
    std::string c = patchName;
    m_pWADLoader->load_patch(c);
}

AssetsManager *AssetsManager::GetInstance(){
    if(instance == nullptr)
        instance = new AssetsManager();
    return instance;
}
