#include "AssetsManager.h"
#include <algorithm>

AssetsManager* AssetsManager::instance =nullptr;


AssetsManager::AssetsManager(){}


AssetsManager::~AssetsManager(){
    for (auto patch = m_PatchCache.begin(); patch != m_PatchCache.end(); patch++)
        delete patch->second;
}


void AssetsManager::Init(wad_loader *pWADLoader){
    m_pWADLoader = pWADLoader;
    LoadTextures();
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

Texture *AssetsManager::AddTexture(WADTextureData &textureData){
    m_TextureCache[textureData.TextureName] = new Texture(textureData);
    Texture *pTexture = m_TextureCache[textureData.TextureName];
    return pTexture;
}


Texture *AssetsManager::GetTexture(const std::string &sTextureName){
    if(!m_TextureCache.count(sTextureName)) return nullptr;
    Texture *pTexture = m_TextureCache[sTextureName];
    if(!pTexture->IsComposed()) pTexture->Compose();
    return pTexture;
}

std::string AssetsManager::GetPName(int iPName){
    auto r = m_PNameLookUp[iPName];
    std::transform(r.begin(), r.end(), r.begin(), ::toupper);
    return r;
}

void AssetsManager::AddPName(std::string &sPName){
    m_PNameLookUp.push_back(sPName);
}

void AssetsManager::LoadTextures(){
    m_pWADLoader->load_pnames();
    m_pWADLoader->load_texture("TEXTURE1");
    m_pWADLoader->load_texture("TEXTURE2");
}

AssetsManager *AssetsManager::GetInstance(){
    if(instance == nullptr)
        instance = new AssetsManager();
    return instance;
}
