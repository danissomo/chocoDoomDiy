#include <SDL2/SDL.h>



class Game{
    public:
    static Game* GetInstance();
    virtual ~Game();

    void ProcessInput();
    void Render();
    void Update();
    void Delay();

    bool IsOver();
    bool Init();

    protected:
    static Game* gameInst; 
    Game();   
};