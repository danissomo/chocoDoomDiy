#include <SDL2/SDL.h>

#include <string>
#include "map.h"
#include "wad_loader.h"
#include "ViewRenderer.h"
#include "DisplayManager.h"
class DoomEngine {
 public:
  DoomEngine();
  ~DoomEngine();

  virtual void Render();
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
  void UpdateKeyStates(const Uint8 *keyStates);

 protected:
  int m_iRenderWidth;
  int m_iRenderHeight;

  std::string m_AppName;

  bool m_bIsOver;

  wad_loader m_WADLoader;
  Map *m_pMap;
  Player *m_pPlayer;
  ViewRenderer *m_pViewRenderer;
  DisplayManager *m_pDispManager;
};