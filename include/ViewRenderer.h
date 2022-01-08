#pragma once
#include <SDL2/SDL.h>

#include "data_types.h"
#include "Angle.h"

class ViewRenderer {
  protected:
  class ViewState *viewState;
  class Map *m_pMap;
  class Player *m_pPlayer;
 public:
  ViewRenderer(SDL_Renderer *pRenderer);
  ~ViewRenderer();
   
  void Init(Map *pMap, Player *pPlayer);

  void AddWallInFOV(Seg seg, Angle V1Angle, Angle V2Angle);
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
  
  int AngleToScreen(Angle angle);
  int ScaleTranslate(int offset, int scale, int x_max, float param_cord);
  int RemapX(int x, int offset);
  int RemapY(int y, int offset);

  int autoScaleFactor;
  

  SDL_Renderer *m_pRenderer;
};


