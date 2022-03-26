#pragma once
#include "data_types.h"
#include <string>
#include <vector>
class Patch{
    public:
    Patch(std::string name);
    ~Patch();
    void InitPatch(WADPatchHeader &header);
    void AddPatchCol(WADPatchColumn &col);

    void Render(uint8_t *pFrameBuffer, int iBufPitch, int iX, int iY);


    int GetHeight();
    int GetWidth();
    int GetXOffset();
    int GetYOffset();

    protected:
    int height;
    int width;
    int xOffset;
    int yOffset;

    std::string name;
    std::vector<WADPatchColumn> patchData;
};