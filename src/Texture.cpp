#include "Texture.h"
#include "AssetsManager.h"
Texture::Texture(WADTextureData &textureData){
    m_IsComposed = false;
    m_iOverLapSize = 0;
    m_pOverlapColumnData = nullptr;
    m_iHeight = textureData.Height;
    m_iWidth = textureData.Width;
    m_sNames = textureData.TextureName;
    m_Flags = textureData.Flags;

    for (int i =0; i < textureData.PatchCount; i++)
        m_TexturePatches.push_back(textureData.pTexturePatch[i]);

    m_ColumnPatchCount.resize(textureData.Width, 0);
    m_ColumnIndex.resize(textureData.Width, 0);
    m_ColumnPatch.resize(textureData.Width, 0);
}

Texture::~Texture(){}

bool Texture::IsComposed(){ return m_IsComposed; }

bool Texture::Init(){
    auto assetInst = AssetsManager::GetInstance();
    for(int i =0; i < m_TexturePatches.size(); i++){
        Patch *pPatch = assetInst->GetPatch(assetInst->GetPName(m_TexturePatches[i].PNameIndex));
        int iXStart = m_TexturePatches[i].XOffset;
        int iMaxWidth = iXStart + pPatch->GetWidth() > m_iWidth? m_iWidth : iXStart + pPatch->GetWidth();

        int iXIndex = iXStart < 0? 0: iXStart;
        while(iXIndex < iMaxWidth){
            m_ColumnPatchCount[iXIndex]++;
            m_ColumnPatch[iXIndex] = i;
            m_ColumnIndex[iXIndex] = pPatch->GetColumnDataIndex(iXIndex - iXStart);
            iXIndex++;
        }
        
        for(int j=0; j< m_iWidth; j++)
            if(m_ColumnPatchCount[j] > 1){
                m_ColumnPatch[j] = -1;
                m_ColumnIndex[j] = m_iOverLapSize;
                m_iOverLapSize += m_iHeight;
            }
    }
    return true;
}

bool Texture::Compose(){
    Init();
    m_pOverlapColumnData = new uint8_t[m_iOverLapSize];
    auto pAssetInst = AssetsManager::GetInstance();
    for(int i=0; i< m_TexturePatches.size(); i++){
        Patch *pPatch = pAssetInst->GetPatch(pAssetInst->GetPName(m_TexturePatches[i].PNameIndex));

        int iXStart = m_TexturePatches[i].XOffset;

        int iMaxWidth = iXStart + pPatch->GetWidth() > m_iWidth? m_iWidth : iXStart + pPatch->GetWidth();
        int iXIndex = iXStart < 0 ? 0 : iXStart;

        while (iXIndex < iMaxWidth){
            if(m_ColumnPatch[iXIndex] < 0){
                int iPatchColumn = pPatch->GetColumnDataIndex(iXIndex - iXStart);
                pPatch->ComposeColumn(m_pOverlapColumnData, m_iHeight, iPatchColumn, m_ColumnIndex[iXIndex], m_TexturePatches[i].YOffset);
            }
            iXIndex++; 
        }
          
    }
    m_IsComposed = true;
    return m_IsComposed;
}

void Texture::Render(uint8_t *pScreenbuffer, int iBufferPitch, std::pair<int, int> XYScreenLoaction){
    for(size_t iCurCol = 0; iCurCol < m_iWidth; iCurCol++)
        RenderColumn(pScreenbuffer, 
                     iBufferPitch, 
                     {XYScreenLoaction.first + iCurCol, XYScreenLoaction.second}, 
                     iCurCol);
}

void Texture::RenderColumn(uint8_t *pScreenbuffer, int iBufferPitch, std::pair<int, int>  XYScreenLoaction, int iCurrentColumnIndex){
    auto pAssetInst = AssetsManager::GetInstance();
    if(m_ColumnPatch[iCurrentColumnIndex] > -1){
        Patch *pPatch =pAssetInst->GetPatch(pAssetInst->GetPName(m_TexturePatches[m_ColumnPatch[iCurrentColumnIndex]].PNameIndex));
        pPatch->RenderColumn(pScreenbuffer, 
                             iBufferPitch, 
                             m_ColumnIndex[iCurrentColumnIndex], 
                             XYScreenLoaction, 
                             m_iHeight, 
                             m_TexturePatches[m_ColumnPatch[iCurrentColumnIndex]].YOffset);
    }else 
        for(int iY = 0; iY < m_iHeight; iY++)
            pScreenbuffer[iBufferPitch*(XYScreenLoaction.second + iY) + XYScreenLoaction.first] = m_pOverlapColumnData[m_ColumnIndex[iCurrentColumnIndex] + iY];
}

void Texture::RenderColumnWithScale(uint8_t* pScreenbuffer, int iBufferPitch, int iColumn, int XCur, int YStart, int YEnd, int tYStart, int tYEnd){
    auto pAssetInst = AssetsManager::GetInstance();
    float textureY = tYStart;
    float step = float(tYEnd - tYStart)/float(YEnd - YStart);
    if(m_ColumnPatch[iColumn] > -1){
        Patch *pPatch =pAssetInst->GetPatch(pAssetInst->GetPName(m_TexturePatches[m_ColumnPatch[iColumn]].PNameIndex));
        pPatch->RenderColumnWithScale(pScreenbuffer, 
                                      iBufferPitch, 
                                      m_ColumnIndex[iColumn],
                                      XCur,
                                      YStart,
                                      YEnd,
                                      m_TexturePatches[m_ColumnPatch[iColumn]].YOffset, 
                                      tYStart,
                                      tYEnd);
    }else 
        for(int iY = YStart; textureY<  m_iHeight && iY < YEnd; iY++, textureY+=step){
            pScreenbuffer[iBufferPitch* iY + XCur] = m_pOverlapColumnData[m_ColumnIndex[iColumn] + int(textureY)];
            
            }
}


int Texture::GetWidth(){return m_iWidth;}
int Texture::GetHeight(){return m_iHeight;}
