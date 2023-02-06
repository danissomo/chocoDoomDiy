#pragma once
#include "data_types.h"

#include "Patch.h"
#include <vector>
enum EWEAPONTYPE{
    eMELEE,
    eBULLET,
    eAREA
};

class Weapon{

    public:
    Weapon();
    virtual ~Weapon();

    void Render(uint8_t *pScreenBuffer, int iBufferPitch);
    void Update();

    void FirePressed();
    void FireReleased();
    bool Fire();
    int GetAmmo();
    int AddAmmo(int count);
    bool IsFiring();
    static EWEAPONTYPE GetWeapontype();

    protected:
    bool buttonState = false; 
    bool canFire = true;
    bool isFiring;
    int ammoCount = 0;
    int lastUpdateTime;
    static int animFrameTime;
    static EWEAPONTYPE wType;
    static bool isAuto;


    int curFrame; 
    static std::vector<std::vector<std::string>> animationNames;
    std::vector<std::vector<Patch *>> animationPatches;
};


class Pistol: public Weapon{};


