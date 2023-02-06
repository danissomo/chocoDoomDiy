#pragma once
#include "data_types.h"


#include <string>
#include <vector>
#include <utility>
class Patch{
    public:
    Patch(std::string name);
    ~Patch();
    void InitPatch(WADPatchHeader &header);
    void AddPatchCol(WADPatchColumn &col);

    void Render(uint8_t *pFrameBuffer, int iBufPitch, int iX, int iY);
    void RenderColumn(uint8_t *pScreenBuffer, int iBufferPitch, int iColumn, std::pair<int, int> XYLoc, int iMaxHeight, int iYOffset);
    void RenderColumnWithScale(uint8_t* pScreenBuffer, int iBufferPitch, int iColumn, int XCur, int YStart, int YEnd, int iYOffset,  int tYStart, int tYEnd);
    void RenderLineWithScale(uint8_t* pScreenBuffer, int iBufferPitch, int iRow, int X1, int Y1, int X2, int Y2, int iYOffset, int tXStart, int tXEnd);

    void AppendColumnStartIndex();
    void ComposeColumn(uint8_t *pOverLapColumnData, int iHeight, int &iPatchColumnIndex, int iColumnOffsetIndex, int iYOrigin);

    int GetHeight();
    int GetWidth();
    int GetXOffset();
    int GetYOffset();
    int GetColumnDataIndex(int index);

    protected:
    int height;
    int width;
    int xOffset;
    int yOffset;

    std::string name;
    std::vector<WADPatchColumn> patchData;
    std::vector<int> m_ColumnIndex;
};