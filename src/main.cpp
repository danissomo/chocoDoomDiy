#include <iostream>
#include <random>
#include "Game.h"
#include "time.h"
int main(int argc, char* argv[]){
    Game game;
    game.Init();
    srand(time(0));
    while (!game.IsOver()){
        game.ProcessInput();
        game.Update();
        game.Render();
        game.Delay();
    }

    return 0;
}