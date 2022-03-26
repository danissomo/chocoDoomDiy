#include "Patch.h"

Patch::Patch(std::string name){
    this->name = name;
}

Patch::~Patch(){

}

void Patch::InitPatch(WADPatchHeader &header){
    width = header.width;
    height = header.height;
    xOffset = header.leftOffset;
    yOffset = header.topOffset;
}

void Patch::AddPatchCol(WADPatchColumn &col){
    patchData.push_back(col);
}


void Patch::Render(uint8_t *pFrameBuffer, int iBufPitch, int iX, int iY){
    int iXIndex = 0;
    for(auto col = patchData.begin(); col!=patchData.end(); col++){
        if(col->topDelta == 0xFF){
            iXIndex++;
            continue;
        }
        for(int iYindex =0; iYindex < col->len; iYindex++){
            auto y = iY + col->topDelta + iYindex;
            auto x = iX + iXIndex;
            pFrameBuffer[320*y + x] = col->pColumnData[iYindex];
        }
    }
}

int Patch::GetHeight(){
    return height;
}
int Patch::GetWidth(){
    return width;
}
int Patch::GetXOffset(){
    return xOffset;
}
int Patch::GetYOffset(){
    return yOffset;
}