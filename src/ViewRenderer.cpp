#include "ViewRenderer.h"
#include "map.h"
class ViewState {
 public:
  virtual void Render(ViewRenderer *v) = 0;
  virtual bool SetAutoMap(ViewRenderer *v) { return false; }
  virtual bool Set3DView(ViewRenderer *v) { return false; }
};

class c_StateAutoMap : public ViewState {
 public:
 
  c_StateAutoMap() {}
  ~c_StateAutoMap() {}
  void Render(ViewRenderer *v) { v->RenderAutoMap(); }
  bool Set3DView(ViewRenderer *v);
};

class c_State3DRender : public ViewState {
 public:
  c_State3DRender() {}
  ~c_State3DRender() {}
  void Render(ViewRenderer *v) { v->Render3DView(); }
  bool SetAutoMap(ViewRenderer *v) {
    v->SetCurrentState(new c_StateAutoMap());
    delete this;
    return true;
  }
};

bool c_StateAutoMap::Set3DView(ViewRenderer *v){
  v->SetCurrentState(new c_State3DRender());
    delete this;
    return true;
}

ViewRenderer::ViewRenderer(SDL_Renderer *pRenderer)
    : m_pRenderer(pRenderer), autoScaleFactor(1) {}

ViewRenderer::~ViewRenderer() {}

void ViewRenderer::Init(Map *pMap, Player *pPlayer) {
  viewState = new c_State3DRender();
  m_pPlayer = pPlayer;
  m_pMap = pMap;
}

void ViewRenderer::AddWallInFOV(Seg seg, Angle V1Angle, Angle V2Angle) {
  int x_size, y_size;
  SDL_RenderGetLogicalSize(m_pRenderer, &x_size, &y_size);
  int v1xscreen = AngleToScreen(V1Angle);
  int v2xscreen = AngleToScreen(V2Angle);

  SDL_RenderDrawLine(m_pRenderer, v1xscreen, 0, v1xscreen, y_size);
  SDL_RenderDrawLine(m_pRenderer, v2xscreen, 0, v2xscreen, y_size);
}

int ViewRenderer::AngleToScreen(Angle angle) {
  int iX = 0;

  // Left side
  if (angle > 90) {
    angle -= 90;
    iX = 160 - round(tanf(angle.GetValue() / 57.2957795) * 160);
  } else {
    // Right side
    angle = 90 - angle.GetValue();
    iX = round(tanf(angle.GetValue() / 57.2957795) * 160);
    iX += 160;
  }

  return iX;
}

int ViewRenderer::ScaleTranslate(int offset, int scale, int x_max,
                                 float param_cord) {
  return offset + x_max / 2 + (-x_max / 2 + x_max * param_cord) / scale;
}

int ViewRenderer::RemapX(int x, int offset) {
  int iRenderXSize;
  int iRenderYSize;

  // Read what is the resolution we are using
  SDL_RenderGetLogicalSize(m_pRenderer, &iRenderXSize, &iRenderYSize);
  iRenderYSize--;
  iRenderXSize--;
  return ScaleTranslate(
      offset, autoScaleFactor, iRenderXSize,
      float(x - m_pMap->GetXmin()) / (m_pMap->GetXmax() - m_pMap->GetXmin()));
}

int ViewRenderer::RemapY(int y, int offset) {
  int iRenderXSize;
  int iRenderYSize;

  // Read what is the resolution we are using
  SDL_RenderGetLogicalSize(m_pRenderer, &iRenderXSize, &iRenderYSize);
  iRenderYSize--;
  iRenderXSize--;
  return ScaleTranslate(
      offset, autoScaleFactor, iRenderYSize,
      float(-y + m_pMap->GetYmax()) / (m_pMap->GetYmax() - m_pMap->GetYmin()));
}

void ViewRenderer::InitFrame() {
  SetDrawColor(0, 0, 0);
  SDL_RenderClear(m_pRenderer);
}

void ViewRenderer::SetDrawColor(int R, int G, int B) {
  SDL_SetRenderDrawColor(m_pRenderer, R, G, B, SDL_ALPHA_OPAQUE);
}

void ViewRenderer::DrawRect(int X1, int Y1, int X2, int Y2) {
  SDL_Rect Rect = {RemapX(X1, 0), RemapY(Y1, 0),
                   RemapX(X2, 0) - RemapX(X1, 0) + 1,
                   RemapY(Y2, 0) - RemapY(Y1, 0) + 1};
  SDL_RenderDrawRect(m_pRenderer, &Rect);
}

void ViewRenderer::DrawLine(int X1, int Y1, int X2, int Y2) {
  SDL_RenderDrawLine(m_pRenderer, RemapX(X1, 0), RemapY(Y1, 0), RemapX(X2, 0),
                     RemapY(Y2, 0));
}

void ViewRenderer::Render() { viewState->Render(this); }

void ViewRenderer::SetCurrentState(ViewState *s) { viewState = s; }

bool ViewRenderer::Set3DView() { return viewState->Set3DView(this); }

bool ViewRenderer::SetAutoMap() { return viewState->SetAutoMap(this); }

void ViewRenderer::RenderAutoMap(){
  m_pMap->RenderAutoMap();
  SetDrawColor(255, 0, 0);
  DrawLine(m_pPlayer->GetX(), m_pPlayer->GetY(), m_pPlayer->GetX() + 5, m_pPlayer->GetY() + 5);
}

void ViewRenderer::Render3DView(){
  m_pMap->RenderBSPNodes();
}

