#pragma once
#include <SDL2/SDL.h>
#include "Angle.h"
#include "data_types.h"
class Player{
    public:
    Player(int ID);
    ~Player();
    
    void SetX(int);
    void SetY(int);
    void SetZ(int);
    void SetAngle(int);
    
    int GetID();
    int GetX();
    int GetY();
    int GetZ();

    int GetFov();
    Angle GetAngle();

    bool IsLineInFOV(Vertex &v1, Vertex &v2, Angle &v1_angle, Angle &v2_angle, Angle &v1AnglefPl, Angle &v2AnglefPl);
    
    void RotateLeft();
    void RotateRight();
    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();

    float DistanceToPoint(Vertex &v);
   
    protected:
    int ID;
    int plX;
    int plY;
    int plZ;
    int pl_FOV;
    int pl_rot_speed;
    int pl_mov_speed;
    Angle plAngle;
    Angle AngleOfVertexInFOV(Vertex &v);
};

