#include "DoomEngine.h"
#include <iostream>
DoomEngine::DoomEngine(SDL_Renderer *pRenderer)
    : m_bIsOver(false), m_iRenderWidth(320), m_iRenderHeight(200) {
  m_pRenderer =  pRenderer;
}

DoomEngine::~DoomEngine() { delete m_pMap; }

bool DoomEngine::Init() {
  m_pViewRenderer = new ViewRenderer(m_pRenderer);
  m_pPlayer = new Player(THINGTYPE::ePLAYER);
  m_pMap = new Map("E1M1", m_pPlayer);

  m_pViewRenderer->Init(m_pMap, m_pPlayer);

  m_WADLoader.set_wad_path(GetWADFileName());

  std::cout << "Info: Loading WAD " << GetWADFileName() << std::endl;

  if ( !m_WADLoader.load_wad() ){
    std::cout << "Error: failed WAD loading" << std::endl;
    return false;
  }

  std::cout << "Info: loading map data " << m_pMap->get_name() << std::endl;

  if( !m_WADLoader.load_map_data(m_pMap) ){
    std::cout << "Error: failed to load map "<< std::endl;
    return false;
  }
  m_pMap->Init();
  return true;
}

std::string DoomEngine::GetWADFileName() { return "DOOM.WAD"; }

void DoomEngine::Render(SDL_Renderer *pRenderer) {
  SDL_SetRenderDrawColor(pRenderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(pRenderer);
  m_pViewRenderer->Render();
}

void DoomEngine::KeyPressed(SDL_Event &event) {
  switch (event.key.keysym.sym) {
    case SDLK_UP:
      break;

    case SDLK_DOWN:
      break;

    case SDLK_LEFT:
      //m_pPlayer->RotateLeft();
      break;

    case SDLK_RIGHT:
      //m_pPlayer->RotateRight();
      break;

    case SDLK_ESCAPE:
      Quit();
      break;
    case SDLK_TAB:
      if( m_pViewRenderer->viewState->IsAutoMap()){
          m_pViewRenderer->viewState->Set3DView(m_pViewRenderer);
      }else{
        m_pViewRenderer->viewState->SetAutoMap(m_pViewRenderer);
      }
      break;
    case SDLK_w:
      //m_pPlayer->MoveForward();
      break;
    case SDLK_s:
      //m_pPlayer->MoveBackward();
    default:
      break;
  }
}

void DoomEngine::KeyReleased(SDL_Event &event) {}

void DoomEngine::Quit() { m_bIsOver = true; }

void DoomEngine::Update() {}

bool DoomEngine::IsOver() { return m_bIsOver; }

int DoomEngine::GetRenderWidth() { return m_iRenderWidth; }

int DoomEngine::GetRenderHeight() { return m_iRenderHeight; }

std::string DoomEngine::GetName() { return "DIYDoom"; }

int DoomEngine::GetTimePerFrame() { return 1000 / 60; }

void  DoomEngine::UpdateKeyStates(const Uint8 *keyStates){
  if(keyStates[SDL_SCANCODE_LEFT]){
      m_pPlayer->RotateLeft();
  }
  if(keyStates[SDL_SCANCODE_RIGHT]){
      m_pPlayer->RotateRight();
  }
  if(keyStates[SDL_SCANCODE_W]){
    m_pPlayer->MoveForward();
  }
  if(keyStates[SDL_SCANCODE_S]){
    m_pPlayer->MoveBackward();
  }
  if(keyStates[SDL_SCANCODE_A]){
    m_pPlayer->MoveLeft();
  }
  if(keyStates[SDL_SCANCODE_D]){
    m_pPlayer->MoveRight();
  }
}
