#include <SDL2/SDL.h>

#include <string>
#include "map.h"
#include "wad_loader.h"

class DoomEngine {
 public:
  DoomEngine(SDL_Renderer *pRenderer);
  ~DoomEngine();

  virtual void Render(SDL_Renderer *);
  virtual void KeyPressed(SDL_Event &);
  virtual void KeyReleased(SDL_Event &);
  virtual void Quit();
  virtual void Update();

  virtual bool IsOver();
  virtual bool Init();

  virtual int GetRenderWidth();
  virtual int GetRenderHeight();
  virtual int GetTimePerFrame();

  virtual std::string GetName();
  virtual std::string GetWADFileName();

 protected:
  int m_iRenderWidth;
  int m_iRenderHeight;

  bool m_bIsOver;

  wad_loader m_WADLoader;
  Map *m_pMap;
  Player *m_pPlayer;

  SDL_Renderer *m_pRenderer;
};