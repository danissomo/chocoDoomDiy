#include <SDL2/SDL.h>

#include "data_types.h"
#include "map.h"
#include <list>
#include <map>
#include<vector>


class ViewRenderer {
  protected:
  
  Map *m_pMap;
  Player *m_pPlayer;
 public:
  ViewRenderer();
  ~ViewRenderer();
   
  void Init(Map *pMap, Player *pPlayer, int Xsize = 320, int Ysize = 200);

  void AddWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle, Angle v1AngleFpl, Angle V2AngleFpl);
  void InitFrame();
  void SetDrawColor(int R, int G, int B);
  void DrawRect(int X1, int Y1, int X2, int Y2);
  void DrawLine(int X1, int Y1, int X2, int Y2);

  void DrawVerticalLine(int iX, int iStartY, int iEndY, uint8_t color);

  void RenderAutoMap();
  void Render3DView();
  void Render(uint8_t *pScreenBuffer, int iBufferPitch);
  

  

 protected:
  struct SolidSegmentRange{
        int XStart;
        int XEnd;
    };

  struct SolidSegmentData{
        Seg &seg;
        int XStart;
        int XEnd;
    };
  struct SegmentRenderData{
        int V1XScreen;
        int V2XScreen;
        Angle V1Angle;
        Angle V2Angle;
        float DistanceToV1;
        float DistanceToNormal;
        float V1ScaleFactor;
        float V2ScaleFactor;
        float Steps;

        float RightSectorCeiling;
        float RightSectorFloor;
        float CeilingStep;
        float CeilingEnd;
        float FloorStep;
        float FloorStart;

        float LeftSectorCeiling;
        float LeftSectorFloor;

        bool bDrawUpperSection;
        bool bDrawLowerSection;

        float UpperHeightStep;
        float iUpperHeight;
        float LowerHeightStep;
        float iLowerHeight;

        bool UpdateFloor;
        bool UpdateCeiling;

		    Seg *pSeg;
    };
  

  int AngleToScreen(Angle angle);
  float GetScaleFactor(int vxScreen, Angle segToNormalAngle, float distToNormal);

  
  int X_screen_size;
  int Y_screen_size;
  int X_half_screen_size;
  int Y_half_screen_size;
  int m_DistPlayerToScreen;
  int m_iBufferPitch;

  std::map<int, Angle> m_ScreenXToAngle;

  uint8_t *m_pScreenBuffer;
  
  //SDL_Renderer *m_pRenderer;

  std::list<SolidSegmentRange> m_SolidWallRanges;
  std::map<std::string, uint8_t> m_WallColor;
  std::vector<int> m_FloorClipHeight;
  std::vector<int> m_CeilingClipHeight;

  uint8_t GetWallColor(std::string textureName);
  bool ValidateRange(SegmentRenderData &renderData, int &iXCur, int  &curCeilingEnd, int &curFloorStart);

  void StoreWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle v1Angle, Angle v2Angle);
  void ClipSolidWall(Seg &seg, int V1Xscreen, int V2Xscreen, Angle v1Angle, Angle v2Angle);
  void ClipPassWall (Seg &seg, int V1XScreen, int V2XScreen, Angle v1Angle, Angle v2Angle);
  void CeilingFloorUpdate(SegmentRenderData &renderData);
  void RenderSegment(SegmentRenderData &renderData);

  void DrawMiddleSection(SegmentRenderData &renderData, int iXcur, int curCeilingEnd, int curFloorStart);
  void DrawUpperSection(SegmentRenderData &renderData, int iXcur, int curCeilingEnd);
  void DrawLowerSection(SegmentRenderData &renderData, int iXcur, int curFloorStart);

  
  void CalculateWallHeight(Seg &seg, int v1X, int v2X, Angle v1Angle, Angle v2Angle );
  void PartialSeg(Seg &seg, Angle& v1angle, Angle& v2angle, float &dist, bool isLeftSide);
  void CalculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen);
};


