#include "ViewRenderer.h"
#include "map.h"
#include <bits/stdc++.h>

//state segment


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

void ViewRenderer::SetCurrentState(ViewState *s) { viewState = s; }

bool ViewRenderer::Set3DView() { return viewState->Set3DView(this); }

bool ViewRenderer::SetAutoMap() { return viewState->SetAutoMap(this); }



//constructors and destructors


ViewRenderer::ViewRenderer(SDL_Renderer *pRenderer)
    : m_pRenderer(pRenderer), autoScaleFactor(1) {}

ViewRenderer::~ViewRenderer() {}


//helping functions segment


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

int ViewRenderer::RemapX(int x, int offset) {
  return ScaleTranslate(
      offset, autoScaleFactor, X_screen_size-1,
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

int ViewRenderer::ScaleTranslate(int offset, int scale, int x_max, float param_cord) {
  return offset + x_max / 2 + (-x_max / 2 + x_max * param_cord) / scale;
}

SDL_Color ViewRenderer::GetWallColor(std::string textureName){
  if (m_WallColor.count(textureName))
    return m_WallColor[textureName];
  else{
    SDL_Color color { u_char(rand() % 255), u_char(rand() % 255), u_char(rand() % 255) };
    m_WallColor[textureName]=color;
    return color;
  }
  
}


int ViewRenderer::AngleToScreen(Angle angle) {
  int iX = 0;

  // Left side
  if (angle.GetValue() > 90) {
    angle -= 90;
    iX = m_DistPlayerToScreen - round(angle.GetTanVal() *X_half_screen_size);
  } else {
    // Right side
    angle = 90 - angle.GetValue();
    iX = round(angle.GetTanVal() * X_half_screen_size);
    iX += m_DistPlayerToScreen;
  }

  return iX;
}

//render segment


void ViewRenderer::Init(Map *pMap, Player *pPlayer) {
  SDL_RenderGetLogicalSize(m_pRenderer, &X_screen_size, &Y_screen_size);
  viewState = new c_State3DRender();
  m_pPlayer = pPlayer;
  m_pMap = pMap;

  Angle HalfFOV= m_pPlayer->GetFov()/2;
  X_half_screen_size = X_screen_size/2;
  Y_half_screen_size = Y_screen_size/2;
  m_DistPlayerToScreen = X_half_screen_size/HalfFOV.GetTanVal();

  for (int i = 0; i <= X_screen_size; i++)
      m_ScreenXToAngle[i] = atan((X_half_screen_size-i) / float(m_DistPlayerToScreen))*180/PI;
}

//must be called before render every frame
void ViewRenderer::InitFrame() {

  // Read what is the resolution we are using
  SDL_RenderGetLogicalSize(m_pRenderer, &X_screen_size, &Y_screen_size);

  Angle HalfFOV= m_pPlayer->GetFov()/2;
  X_half_screen_size = X_screen_size/2;
  Y_half_screen_size = Y_screen_size/2;
  m_DistPlayerToScreen = X_half_screen_size/HalfFOV.GetTanVal();
  SetDrawColor(0, 0, 0);
  SDL_RenderClear(m_pRenderer);

  m_SolidWallRanges.clear();

  SolidSegmentRange wallLeftSide;
  SolidSegmentRange wallRightSide;

  wallLeftSide.XStart = INT_MIN;
  wallLeftSide.XEnd   = -1;
  m_SolidWallRanges.push_back(wallLeftSide);


  wallRightSide.XStart = X_screen_size;
  wallRightSide.XEnd   = INT_MAX;
  m_SolidWallRanges.push_back(wallRightSide);
}

void ViewRenderer::Render() { viewState->Render(this); }

void ViewRenderer::RenderAutoMap(){
  m_pMap->RenderAutoMap();
  SetDrawColor(255, 0, 0);
  DrawLine(m_pPlayer->GetX(), m_pPlayer->GetY(), m_pPlayer->GetX() + 5, m_pPlayer->GetY() + 5);
}

void ViewRenderer::Render3DView(){
  InitFrame();
  m_pMap->RenderBSPNodes();
}


//walls segment


void ViewRenderer::AddWallInFOV(Seg &seg, Angle V1Angle, Angle V2Angle, Angle v1AngleFpl, Angle V2AngleFpl) {
  int v1xscreen = AngleToScreen(v1AngleFpl);
  int v2xscreen = AngleToScreen(V2AngleFpl);
  if (v1xscreen == v2xscreen) return;
  if (seg.pLeftSector == nullptr) 
        ClipSolidWall(seg, v1xscreen, v2xscreen, V1Angle, V2Angle);
   
}


void ViewRenderer::StoreWallRange(Seg &seg, int V1XScreen, int V2XScreen, Angle v1Angle, Angle v2Angle){
  SolidSegmentData Wall = { seg, V1XScreen, V2XScreen };
  CalculateWallHeight(seg, V1XScreen, V2XScreen, v1Angle, v2Angle);
  //DrawSolidWall(Wall);
}

void ViewRenderer::DrawSolidWall(SolidSegmentData &wall){
  SDL_Color color = GetWallColor(wall.seg.pLinedef->pRightSidedef->MiddleTexture);
    // We know nothing about the wall hight yet, so draw from top of the screen to the bottom
  SDL_Rect Rect = { wall.XStart, 0, wall.XEnd - wall.XStart + 1, Y_screen_size };
  SetDrawColor(color.r, color.g, color.b);
  SDL_RenderFillRect(m_pRenderer, &Rect);
}

void ViewRenderer::ClipSolidWall(Seg &seg, int V1XScreen, int V2XScreen, Angle v1Angle, Angle v2Angle){
  // Find clip window 
  if (m_SolidWallRanges.size() < 2)
      return;

    SolidSegmentRange CurrentWall = { V1XScreen, V2XScreen };

    std::list<SolidSegmentRange>::iterator FoundClipWall = m_SolidWallRanges.begin();
    while (FoundClipWall != m_SolidWallRanges.end() && FoundClipWall->XEnd < CurrentWall.XStart - 1){
        ++FoundClipWall;
    }

    if (CurrentWall.XStart < FoundClipWall->XStart){
        // Are the edges touching?
        if (CurrentWall.XEnd < FoundClipWall->XStart - 1){
            //All of the wall is visible, so insert it
            StoreWallRange(seg, CurrentWall.XStart, CurrentWall.XEnd, v1Angle, v2Angle);
            m_SolidWallRanges.insert(FoundClipWall, CurrentWall);
            return;
        }

        // The end is already included, just update start
        StoreWallRange(seg, CurrentWall.XStart, FoundClipWall->XStart - 1, v1Angle, v2Angle);
        FoundClipWall->XStart = CurrentWall.XStart;
    }

    // This part is already occupied
    if (CurrentWall.XEnd <= FoundClipWall->XEnd)
        return;

    std::list<SolidSegmentRange>::iterator NextWall = FoundClipWall;

    while (CurrentWall.XEnd >= next(NextWall, 1)->XStart - 1)
    {
        // partially clipped by other walls, store each fragment
        StoreWallRange(seg, NextWall->XEnd + 1, next(NextWall, 1)->XStart - 1, v1Angle, v2Angle);
        ++NextWall;

        if (CurrentWall.XEnd <= NextWall->XEnd){
            FoundClipWall->XEnd = NextWall->XEnd;
            if (NextWall != FoundClipWall){
                //Delete a range of walls
                FoundClipWall++;
                NextWall++;
                m_SolidWallRanges.erase(FoundClipWall, NextWall);
            }
            return;
        }
    }

    StoreWallRange(seg, NextWall->XEnd + 1, CurrentWall.XEnd, v1Angle, v2Angle);
    FoundClipWall->XEnd = CurrentWall.XEnd;

    if (NextWall != FoundClipWall){
        FoundClipWall++;
        NextWall++;
        m_SolidWallRanges.erase(FoundClipWall, NextWall);
    }

}

void ViewRenderer::CalculateWallHeight(Seg &seg, int v1X, int v2X, Angle v1Angle, Angle v2Angle ){
  float distToV1 = m_pPlayer->DistanceToPoint(*(seg.pStartVertex));
  float distToV2 = m_pPlayer->DistanceToPoint(*(seg.pEndVertex));

  if( v1X <= 0){
    //std::cout << "partial left\n";
    PartialSeg(seg, v1Angle, v2Angle, distToV1, true);
  }
  if(v2X >= X_screen_size-1){
    //std::cout << "partial right\n";
    PartialSeg(seg, v1Angle, v2Angle, distToV2, false);
  }
  float ceilingV1_screen=0, floorV1_screen=0;
  float ceilingV2_screen=0, floorV2_screen=0;
  CalculateCeilingFloorHeight(seg, v1X, distToV1, ceilingV1_screen, floorV1_screen);
  CalculateCeilingFloorHeight(seg, v2X, distToV2, ceilingV2_screen, floorV2_screen);
  auto color = GetWallColor(seg.pLinedef->pRightSidedef->MiddleTexture);
  SetDrawColor(color.r, color.g, color.b);

  SDL_RenderDrawLine(m_pRenderer, v1X, ceilingV1_screen, v1X, floorV1_screen);
  SDL_RenderDrawLine(m_pRenderer, v2X, ceilingV2_screen, v2X, floorV2_screen);
  SDL_RenderDrawLine(m_pRenderer, v1X, ceilingV1_screen, v2X, ceilingV2_screen);
  SDL_RenderDrawLine(m_pRenderer, v1X, floorV1_screen, v2X, floorV2_screen);

}

void  ViewRenderer::PartialSeg(Seg &seg, Angle& v1angle, Angle& v2angle, float &dist, bool isLeftSide){
  float SideC = sqrt(pow(seg.pStartVertex->X_pos - seg.pEndVertex->X_pos, 2) + pow(seg.pStartVertex->Y_pos - seg.pEndVertex->Y_pos, 2));
     Angle V1toV2Span;
     V1toV2Span = v1angle - v2angle;
    float SINEAngleB = dist * V1toV2Span.GetSinVal() / SideC;
    Angle AngleB(asinf(SINEAngleB) * 180.0 / PI);
    Angle AngleA(180 - V1toV2Span.GetValue() - AngleB.GetValue());

    Angle AngleVToFOV;
    if (isLeftSide){
        AngleVToFOV = v1angle - (m_pPlayer->GetAngle() + 45);
    }else{
        AngleVToFOV = (m_pPlayer->GetAngle() - 45) - v2angle;
    }

    Angle NewAngleB(180 - AngleVToFOV.GetValue() - AngleA.GetValue());
    dist = (dist * AngleA.GetSinVal() )/ NewAngleB.GetSinVal();
}

void ViewRenderer::CalculateCeilingFloorHeight(Seg &seg, int &VXScreen, float &DistanceToV, float &CeilingVOnScreen, float &FloorVOnScreen){
  float ceiling = seg.pRightSector->CeilingHeight- m_pPlayer->GetZ();
  float Floor = seg.pRightSector->FloorHeight - m_pPlayer->GetZ();

  Angle vscreenAngle = m_ScreenXToAngle[VXScreen];
  float distToVScreen = m_DistPlayerToScreen/vscreenAngle.GetCosVal();//change to getter distance to screen 
  CeilingVOnScreen = abs(ceiling)*distToVScreen/ DistanceToV;
  FloorVOnScreen = abs(Floor)*distToVScreen/DistanceToV;
  if(ceiling > 0 )
    CeilingVOnScreen = Y_half_screen_size - CeilingVOnScreen;
  else
    CeilingVOnScreen += Y_half_screen_size;
  if (Floor > 0)
    FloorVOnScreen = Y_half_screen_size - FloorVOnScreen;
  else
    FloorVOnScreen += Y_half_screen_size;
}

