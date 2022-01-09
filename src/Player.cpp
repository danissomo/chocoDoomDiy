

#include "Player.h"
#include <cmath>
Player::Player( int ID){
    this->ID        = ID;
    pl_FOV           = 90;
    pl_mov_speed    = 2;
    pl_rot_speed    = 4;
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

void Player::SetAngle(int A){
    plAngle = A;
}

void Player::SetX(int X){
    plX = X;
}

void Player::SetY(int Y){
    plY = Y;
}

Angle Player::AngleOfVertexInFOV(Vertex &v){
    int vec_x = v.X_pos - plX;
    int vec_y = v.Y_pos - plY;
    //180/pi ~= 57.2957795
    return Angle(atan2f(vec_y, vec_x) * 57.2957795);
}

bool Player::IsLineInFOV(Vertex &v1, Vertex &v2, Angle &v1_angle, Angle &v2_angle){
    v1_angle = AngleOfVertexInFOV(v1);
    v2_angle = AngleOfVertexInFOV(v2);
    Angle v1_to_v2_span = v1_angle - v2_angle;
    if(v1_to_v2_span >= 180) return false;
    v1_angle = v1_angle -  plAngle;
    v2_angle = v2_angle -  plAngle;

    Angle FOV_half = pl_FOV/2;

    Angle v1_angle_moved = v1_angle + FOV_half;

    if(v1_angle_moved > pl_FOV){
        Angle v1_angle_moved2 = v1_angle_moved - pl_FOV;
        if ( v1_angle_moved2 >=  v1_to_v2_span) return false;
        v1_angle = FOV_half;
    }

    Angle v2_angle_moved = FOV_half - v2_angle;

    if (v2_angle_moved > pl_FOV)
        v2_angle = v2_angle - FOV_half;

    v1_angle += 90;
    v2_angle += 90;

    return true;
}

void Player::RotateLeft(){
    plAngle += (0.1875f * pl_rot_speed);
}

void Player::RotateRight(){
    plAngle -= (0.1875f * pl_rot_speed);
}
