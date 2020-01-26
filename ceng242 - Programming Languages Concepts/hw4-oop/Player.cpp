#include "Player.h"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Player::Player(uint id, int x, int y):id(id), coordinate(x,y) { }

Player::~Player(){

}

uint Player::getID() const{
    return id;
}

const Coordinate& Player::getCoord() const{
    return coordinate;
}

int Player::getHP() const{
    return HP;
}
void Player::updateHP(int a) {
    HP=HP+a;
}

std::string Player::getBoardID() const{
	std::string x=std::to_string(id);
	if(x.length() ==1) return "0" +x;
    return x;
}

bool Player::isDead() const{
    //bool a= HP> 0 ? false : true ;
    return (HP> 0 ? false : true);
}

void Player::executeMove(Move move){
    coordinate=coordinate + move;
    switch(move) {
            case NOOP:
            case ATTACK:
                break;
            case UP:
                std::cout << getFullName()<<"("<< getHP() << ") moved " ;
                std::cout<<"UP."<<std::endl;
                break;
            case DOWN:
                std::cout << getFullName()<<"("<< getHP() << ") moved " ;
                std::cout<<"DOWN."<<std::endl;
                break;
            case LEFT:
                std::cout << getFullName()<<"("<< getHP() << ") moved " ;
                std::cout<<"LEFT."<<std::endl;
                break;
            case RIGHT:
                std::cout << getFullName()<<"("<< getHP() << ") moved " ;
                std::cout<<"RIGHT."<<std::endl;
                break;
        }
}

bool Player::attackTo(Player *player){
	if(this->getFullName()==player->getFullName()) return false;
	int a=Entity::damageForWeapon(this->weapon)-Entity::damageReductionForArmor(player->armor);
    std::cout<<getFullName()<<"("<< getHP() << ") ATTACKED "<<player->getFullName()<<"("<< player->getHP() << ")! (-"<< std::max(a,0) <<")"<<std::endl;
    player->HP -= std::max(a,0);
    return player->isDead();
}
bool Player::operator < (const Player& pp) const{
    return (id<pp.id );
}