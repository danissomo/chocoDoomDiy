#include "Weapon.h"
#include "AssetsManager.h"
#include <string>
#include <time.h>
Weapon::Weapon(){
    //pPatch = AssetsManager::GetInstance()->GetPatch(sWeaponPatch);
    curFrame = 0;
    for(auto vecFrames = animationNames.begin(); vecFrames != animationNames.end(); vecFrames++){
        std::vector<Patch*> patchBuffer;
        for(auto sFrames = vecFrames->begin(); sFrames != vecFrames->end(); sFrames++){
            auto sBuf = *sFrames;
            patchBuffer.push_back(AssetsManager::GetInstance()->GetPatch(sBuf));
        }
        animationPatches.push_back(patchBuffer);
    }
}

void Weapon::Render(uint8_t *pScreenBuffer, int iBufferPitch){
    for( auto pPatch = animationPatches[curFrame].begin(); pPatch!=animationPatches[curFrame].end(); pPatch++)
        (*pPatch)->Render(pScreenBuffer, iBufferPitch, -(*pPatch)->GetXOffset(), -(*pPatch)->GetYOffset());
}

void Weapon::Update(){
    if (clock() - lastUpdateTime >= Weapon::animFrameTime ){
        if (isFiring){
            curFrame++;
            if (curFrame >= animationPatches.size()){
                isFiring = false;
                curFrame = 0;
                canFire = isAuto || !buttonState;
            
            }
            lastUpdateTime = clock();
        }
    }
}

bool Weapon::Fire(){
    if(canFire){
        isFiring = true;
        return true;
    }
    return false;
}

void Weapon::FirePressed(){
    buttonState = true;
}

void Weapon::FireReleased(){
    buttonState = false;
    if(!isAuto) canFire = true;
}

Weapon::~Weapon(){}

bool Weapon::IsFiring(){
    return isFiring;
}

int Pistol::Weapon::animFrameTime = 20000;
EWEAPONTYPE Pistol::Weapon::wType = EWEAPONTYPE::eBULLET;
bool Pistol::Weapon::isAuto = false;

std::vector<std::vector<std::string>>  Pistol::Weapon::animationNames= {
    {"PISGA0"},
    {"PISGB0", "PISFA0"},
    {"PISGC0"},
    {"PISGD0"},
    //{"PISGE0"}
};
