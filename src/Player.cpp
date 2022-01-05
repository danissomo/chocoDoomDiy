

#include "Player.h"

Player::Player(int ID){
    this->ID = ID;
}

Player::~Player(){

}

int Player::GetAngle(){
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