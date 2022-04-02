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
            pFrameBuffer[iBufPitch*y + x] = col->pColumnData[iYindex];
        }
    }
}

void Patch::RenderColumn(uint8_t *pScreenBuffer,  int iBufferPitch, int iColumn, std::pair<int, int> XYLoc, int iMaxHeight, int iYOffset){
    int iTotalHeight = 0;
    int iYIndex = 0;

    if(iYOffset < 0) iYIndex = iYOffset * -1;

    while(patchData[iColumn].topDelta != 0xFF && iTotalHeight < iMaxHeight){
        while(iYIndex < patchData[iColumn].len && iTotalHeight < iMaxHeight){
            auto y = XYLoc.second + patchData[iColumn].topDelta + iYIndex + iYOffset;
            auto x = XYLoc.first;
            pScreenBuffer[iBufferPitch *y + x] = patchData[iColumn].pColumnData[iYIndex];
            iTotalHeight++;
            iYIndex++;
            
        }
        iColumn++;
        iYIndex = 0;
    }
}

void Patch::RenderColumnWithScale(uint8_t* pScreenBuffer, int iBufferPitch, int iColumn, int XCur, int YStart, int YEnd, int iYOffset,  int tYStart, int tYEnd){
    int iMaxHeight = 200;
    float iTotalHeight = 0;
    float iYIndex = tYStart;
    float step = float(tYEnd - tYStart)/float(YEnd - YStart);
    if(iYOffset < 0) iYIndex = iYOffset * -1;

    while(patchData[iColumn].topDelta != 0xFF && YStart <= YEnd){
        while(int(iYIndex) < patchData[iColumn].len &&  YStart <= YEnd){
            int y = YStart;
            int x = XCur;
            pScreenBuffer[iBufferPitch *y + x] = patchData[iColumn].pColumnData[int(iYIndex)];
            iTotalHeight++;
            iYIndex+=step;
            YStart++;
        }
        iColumn++;
        iYIndex = 0;
    }
}


void Patch::AppendColumnStartIndex(){
    m_ColumnIndex.push_back(patchData.size());
}

void Patch::ComposeColumn(uint8_t *pOverLapColumnData, int iHeight, int &iPatchColumnIndex, int iColumnOffsetIndex, int iYOrigin){
    while ( patchData[iPatchColumnIndex].topDelta != 0xFF){
        int iYPose = iYOrigin + patchData[iPatchColumnIndex].topDelta;
        int iMaxRun = patchData[iPatchColumnIndex].len;

        if(iYPose < 0){
            iMaxRun += iYPose;
            iYPose = 0;
        }
        if(iYPose + iMaxRun > iHeight) iMaxRun= iHeight - iYPose;
        for(int iY = 0; iY < iMaxRun; iY++)
            pOverLapColumnData[iColumnOffsetIndex + iYPose + iY] = patchData[iPatchColumnIndex].pColumnData[iY];
        iPatchColumnIndex++;
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

int Patch::GetColumnDataIndex(int index){
    return m_ColumnIndex[index];
}