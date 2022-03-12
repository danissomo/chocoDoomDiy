#include <SDL2/SDL.h>

#include "DoomEngine.h"

class Game{
    public:
    Game();
    virtual ~Game();

    void ProcessInput();
    void Render();
    void Update();
    void Delay();

    bool IsOver();
    bool Init();

    protected:    
    DoomEngine *m_pDoomEngine;
};