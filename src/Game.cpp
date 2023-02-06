#include "Game.h"

#include <iostream>
#include <string>
#include "DoomEngine.h"

Game::Game() {}

Game * Game::gameInst =nullptr;

Game * Game::GetInstance(){
  if(gameInst == nullptr)
    gameInst =new Game();
  return gameInst;
}

Game::~Game() {
}

bool Game::Init() {
  DoomEngine *pDoomEngine = DoomEngine::GetInstance();
  if(!pDoomEngine || !pDoomEngine->Init()){
    std::cout << "Error: failed to init DoomEngine" << std::endl;
    return false;
  }
  return true;
}

void Game::ProcessInput() {
  DoomEngine *pDoomEngine = DoomEngine::GetInstance();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          pDoomEngine->KeyPressed(event);
          break;

        case SDL_KEYUP:
          pDoomEngine->KeyReleased(event);
          break;

        case SDL_QUIT:
          pDoomEngine->Quit();
          break;
      }
  }
  pDoomEngine->UpdateKeyStates(SDL_GetKeyboardState(NULL));
}

void Game::Render() {
  // Game objects to draw themselves
  DoomEngine::GetInstance()->Render();
}

void Game::Update() { DoomEngine::GetInstance()->Update(); }

bool Game::IsOver() { return DoomEngine::GetInstance()->IsOver(); }

void Game::Delay() { SDL_Delay(DoomEngine::GetInstance()->GetTimePerFrame()); }