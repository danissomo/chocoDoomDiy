#pragma once
#include <SDL2/SDL.h>

#include "data_types.h"
#include "Angle.h"
#include <list>
#include <map>


class ViewRenderer {
  protected:
  class ViewState *viewState;
  class Map *m_pMap;
  class Player *m_pPlayer;
 public:
  ViewRenderer(SDL_Renderer *pRenderer);
  ~ViewRenderer();
   
  void Init(Map *pMap, Player *pPlayer);

  void AddWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle, Angle v1AngleFpl, Angle V2AngleFpl);
  void InitFrame();
  void SetDrawColor(int R, int G, int B);
  void DrawRect(int X1, int Y1, int X2, int Y2);
  void DrawLine(int X1, int Y1, int X2, int Y2);

  void RenderAutoMap();
  void Render3DView();
  void Render();
  void SetCurrentState(ViewState *s);

  bool Set3DView();
  bool SetAutoMap();

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
  int AngleToScreen(Angle angle);
  int ScaleTranslate(int offset, int scale, int x_max, float param_cord);
  int RemapX(int x, int offset);
  int RemapY(int y, int offset);

  int autoScaleFactor;
  int X_screen_size;
  int Y_screen_size;
  int X_half_screen_size;
  int Y_half_screen_size;
  int m_DistPlayerToScreen;

  std::map<int, Angle> m_ScreenXToAngle;

  
  SDL_Renderer *m_pRenderer;

  std::list<SolidSegmentRange> m_SolidWallRanges;
  std::map<std::string, SDL_Color> m_WallColor;

  SDL_Color GetWallColor(std::string textureName);

  void StoreWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle v1Angle, Angle v2Angle);
  void ClipSolidWall(Seg &seg, int V1Xscreen, int V2Xscreen, Angle v1Angle, Angle v2Angle);
  void DrawSolidWall(SolidSegmentData &wall);
  void CalculateWallHeight(Seg &seg, int v1X, int v2X, Angle v1Angle, Angle v2Angle );
  void PartialSeg(Seg &seg, Angle& v1angle, Angle& v2angle, float &dist, bool isLeftSide);
  void CalculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen);
};


