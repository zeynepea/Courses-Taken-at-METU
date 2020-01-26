#include "Tracer.h"
#include <string>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Tracer::Tracer(uint id, int x, int y): Player(id,x,y) {
    HP=100;
    armor = BRICK;
    weapon = SHOVEL;
    PriorityList = { UP, LEFT, DOWN, RIGHT, ATTACK };
    color = Color::FG_YELLOW;
    name = "Tracer";
}
Tracer::~Tracer(){
    
   // PriorityList.clear();

}
Armor Tracer::getArmor() const{
    return armor;
}
Weapon Tracer::getWeapon() const{
    return weapon;
}
std::vector<Move> Tracer::getPriorityList()const{
    return PriorityList;
}
const std::string Tracer::getFullName() const{
    return "Tracer" + getBoardID();
}
Color::Code Tracer::getColorID() const{
	return color;
}
const std::string Tracer::getName() const{
    return name;
}
