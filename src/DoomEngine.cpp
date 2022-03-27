#include "DoomEngine.h"
#include <iostream>
#include "AssetsManager.h"
#include "DisplayManager.h"
DoomEngine::DoomEngine(){
    m_bIsOver = false;
    m_iRenderHeight = 200;
    m_iRenderWidth = 320;
    m_AppName = "DIYDOOM";
}
DoomEngine * DoomEngine::instance = nullptr;
DoomEngine *DoomEngine::GetInstance(){
  if(instance ==nullptr)
    instance = new DoomEngine();
  return instance;
}

DoomEngine::~DoomEngine() { delete m_pMap; }

bool DoomEngine::Init() {
  AssetsManager::GetInstance()->Init(&m_WADLoader);
  DisplayManager::GetInstance()->Init(GetName(), m_iRenderHeight, m_iRenderWidth);

  m_WADLoader.set_wad_path(GetWADFileName());
  

  std::cout << "Info: Loading WAD " << GetWADFileName() << std::endl;

  if ( !m_WADLoader.load_wad() ){
    std::cout << "Error: failed WAD loading" << std::endl;
    return false;
  }
  m_pViewRenderer = new ViewRenderer();
  m_pPlayer = new Player(THINGTYPE::ePLAYER);
  m_pMap = new Map("E1M1", m_pPlayer);
  m_pViewRenderer->Init(m_pMap, m_pPlayer);

  std::cout << "Info: loading map data " << m_pMap->get_name() << std::endl;

  if( !m_WADLoader.load_map_data(m_pMap) ){
    std::cout << "Error: failed to load map "<< std::endl;
    return false;
  }

  if(!m_WADLoader.load_palette()){
    std::cout<< "Error: failed to load palette" << std::endl;
    return false;
  }
  m_pMap->Init();
  
  return true;
}

std::string DoomEngine::GetWADFileName() { return "DOOM.WAD"; }

void DoomEngine::Render() {
  DisplayManager::GetInstance()->InitFrame();
  m_pViewRenderer->Render(DisplayManager::GetInstance()->GetScreenBuffer(), m_iRenderWidth);
  m_pPlayer->Render(DisplayManager::GetInstance()->GetScreenBuffer(), m_iRenderWidth);
  DisplayManager::GetInstance()->Render();
  
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
      
      break;
    case SDLK_w:
      //m_pPlayer->MoveForward();
      break;
    case SDLK_s:
      //m_pPlayer->MoveBackward();
      break;
    case SDLK_SPACE:
      m_pPlayer->FirePressed();
      break;
    default:
      break;
  }
}

void DoomEngine::KeyReleased(SDL_Event &event) {
  switch (event.key.keysym.sym) {
    case SDLK_SPACE:
      m_pPlayer->FireReleased();
    break;
  }
}

void DoomEngine::Quit() { m_bIsOver = true; }

void DoomEngine::Update() {
  m_pPlayer->Think(m_pMap->GetSectorFloorHeight());
  m_pPlayer->Update();
}

bool DoomEngine::IsOver() { return m_bIsOver; }

int DoomEngine::GetRenderWidth() { return m_iRenderWidth; }

int DoomEngine::GetRenderHeight() { return m_iRenderHeight; }

std::string DoomEngine::GetName() { return m_AppName; }

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
  if(keyStates[SDL_SCANCODE_SPACE]){
    m_pPlayer->Fire();
  }
}
