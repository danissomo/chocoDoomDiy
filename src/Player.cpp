

#include "Player.h"
#include <cmath>
Player::Player( int ID){
    this->ID        = ID;
    pl_FOV           = 90;
    pl_mov_speed    = 10;
    pl_rot_speed    = 4;
    plEyeLevel = 41;
    plWeapon = new Pistol();
}

Player::~Player(){

}

Angle Player::GetAngle(){
    return plAngle;
}

int Player::GetX(){
    return plX;
}   

int Player::GetY(){
    return plY;
}

int Player::GetID(){
    return ID;
}

int Player::GetZ(){
    return plZ;
}

void Player::SetAngle(int A){
    plAngle = A;
}

void Player::SetX(int X){
    plX = X;
}

void Player::SetY(int Y){
    plY = Y;
}
void Player::SetZ(int Z){
    plZ = Z;
}

Angle Player::AngleOfVertexInFOV(Vertex &v){
    int vec_x = v.X_pos - plX;
    int vec_y = v.Y_pos - plY;
    //180/pi ~= 57.2957795
    return Angle(atan2f(vec_y, vec_x) * 57.2957795);
}

bool Player::IsLineInFOV(Vertex &v1, Vertex &v2, Angle &v1_angle, Angle &v2_angle, Angle &v1AnglefPl, Angle &v2AnglefPl){
    v1_angle = AngleOfVertexInFOV(v1);
    v2_angle = AngleOfVertexInFOV(v2);
    Angle v1_to_v2_span = v1_angle - v2_angle;
    if(v1_to_v2_span >= 180) return false;
    v1AnglefPl = v1_angle -  plAngle;
    v2AnglefPl = v2_angle -  plAngle;

    Angle FOV_half = pl_FOV/2;

    Angle v1_angle_moved = v1AnglefPl + FOV_half;

    if(v1_angle_moved > pl_FOV){
        Angle v1_angle_moved2 = v1_angle_moved - pl_FOV;
        if ( v1_angle_moved2 >=  v1_to_v2_span) return false;
        v1AnglefPl = FOV_half;
    }

    Angle v2_angle_moved = FOV_half - v2AnglefPl;

    if (v2_angle_moved > pl_FOV)
        v2AnglefPl = - FOV_half;

    v1AnglefPl += 90;
    v2AnglefPl += 90;

    return true;
}

void Player::RotateLeft(){
    plAngle += (0.5f * pl_rot_speed);
}

void Player::RotateRight(){
    plAngle -= (0.5f * pl_rot_speed);
}

void Player::MoveForward(){
    plX += pl_mov_speed*plAngle.GetCosVal();
    plY += pl_mov_speed*plAngle.GetSinVal();
}

void Player::MoveBackward(){
    plX -= pl_mov_speed*plAngle.GetCosVal();
    plY -= pl_mov_speed*plAngle.GetSinVal();
}

void Player::MoveLeft(){
    plY += pl_mov_speed*plAngle.GetCosVal();
    plX -= pl_mov_speed*plAngle.GetSinVal();
}

void Player::MoveRight(){
    plY -= pl_mov_speed*plAngle.GetCosVal();
    plX += pl_mov_speed*plAngle.GetSinVal();
}

float Player::DistanceToPoint(Vertex &v){
    return sqrt(pow(plX- v.X_pos, 2) + pow(plY-v.Y_pos, 2) );
}

int Player::GetFov(){
    return pl_FOV;
}

void Player::Think(int ssFloorHeight){
    plZ = ssFloorHeight + plEyeLevel;
}

void Player::Render(uint8_t *pScreenBuffer, int iBufferPitch){
    plWeapon->Render(pScreenBuffer, iBufferPitch);
}


void Player::Update(){
    plWeapon->Update();
}

bool Player::Fire(){
    return plWeapon->Fire();
}

void Player::FirePressed(){
    plWeapon->FirePressed();
}

void Player::FireReleased(){
    plWeapon->FireReleased();
}
