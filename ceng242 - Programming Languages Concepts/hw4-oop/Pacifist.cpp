#include "Pacifist.h"
#include <string>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Pacifist::Pacifist(uint id, int x, int y): Player(id,x,y) {
    HP = 100;
    armor = METAL;
    weapon = NOWEAPON;
    PriorityList = { UP, LEFT, DOWN, RIGHT };
    color = Color::FG_GREEN;
    name="Pacifist";
}
Pacifist::~Pacifist(){
   
    //PriorityList.clear();

}
Armor Pacifist::getArmor() const{
    return armor;
}
Weapon Pacifist::getWeapon() const{
    return weapon;
}
std::vector<Move> Pacifist::getPriorityList()const{
    return PriorityList;
}
const std::string Pacifist::getFullName() const{
    return "Pacifist" + getBoardID();
}
Color::Code Pacifist::getColorID() const{
	return color;
}
const std::string Pacifist::getName() const{
    return name;
}
