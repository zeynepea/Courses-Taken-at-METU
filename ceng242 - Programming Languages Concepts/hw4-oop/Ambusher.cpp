#include "Ambusher.h"
#include <string>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Ambusher::Ambusher(uint id, int x, int y): Player(id,x,y) {
	
    HP = 100;
    weapon = SEMIAUTO;
    armor = NOARMOR;
    PriorityList = { ATTACK };
    color = Color::FG_BLUE;
    name="Ambusher";
}
Ambusher::~Ambusher(){
   
    //PriorityList.clear();
}
Armor Ambusher::getArmor() const{
    return armor;
}
Weapon Ambusher::getWeapon() const{
    return weapon;
}
std::vector<Move> Ambusher::getPriorityList()const{
    return PriorityList;
}
const std::string Ambusher::getFullName() const{
    return "Ambusher" + getBoardID();
}
Color::Code Ambusher::getColorID() const{
	return color;
}
const std::string Ambusher::getName() const{
    return name;
}

