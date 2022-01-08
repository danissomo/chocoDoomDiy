#pragma once
#include <SDL2/SDL.h>
#include "Angle.h"
#include "data_types.h"
#include "ViewRenderer.h"
class Player{
    public:
    Player(ViewRenderer *pViewRenderer, int ID);
    ~Player();
    
    void SetX(int);
    void SetY(int);
    void SetAngle(int);
    
    int GetID();
    int GetX();
    int GetY();
    Angle GetAngle();

    bool IsLineInFOV(vertex &v1, vertex &v2, Angle &v1_angle, Angle &v2_angle);
    
    void RotateLeft();
    void RotateRight();

    void RenderAutoMap();
    protected:
    int ID;
    int plX;
    int plY;
    int pl_FOV;
    int pl_rot_speed;
    int pl_mov_speed;
    ViewRenderer *m_pViewRenderer;
    Angle plAngle;
    Angle AngleOfVertexInFOV(vertex &v);
};

