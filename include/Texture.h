#pragma once
#include "data_types.h"

#include <string>
#include <vector>
#include <utility>

class Texture{
    public:
    Texture(WADTextureData &textureData);
    ~Texture();

    bool IsComposed();
    bool Init();
    bool Compose();

    void Render(uint8_t *pScreenbuffer, int iBufferPitch, std::pair<int, int> XYScreenLoaction);
    void RenderColumn(uint8_t *pScreenbuffer, int iBufferPitch, std::pair<int, int>  XYScreenLoaction, int iCurrentColumnIndex);
    void RenderColumnWithScale(uint8_t* pScreenbuffer, int iBufferPitch, int iColumn, int XCur, int YStart, int YEnd, int tYStart, int tYEnd);

    int GetWidth();
    int GetHeight();
    protected:
    int m_Flags; //ignored
    int m_iWidth;
    int m_iHeight;
    int m_iOverLapSize;
    
    bool m_IsComposed;

    std::string m_sNames;

    std::vector<int> m_ColumnPatchCount;
    std::vector<int> m_ColumnIndex;
    std::vector<int> m_ColumnPatch;

    std::vector<WADTexturePatch> m_TexturePatches;
    uint8_t *m_pOverlapColumnData;

};