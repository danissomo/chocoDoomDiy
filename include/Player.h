#include <SDL2/SDL.h>

class Player{
    public:
    Player(int ID);
    ~Player();

    void SetX(int);
    void SetY(int);
    void SetAngle(int);
    
    int GetID();
    int GetX();
    int GetY();
    int GetAngle();

    protected:
    int ID;
    int plX;
    int plY;
    int plAngle;

};

