#include "Dummy.h"
#include <string>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Dummy::Dummy(uint id, int x, int y):Player(id,x,y) {
	
    HP = 1000;
    armor = NOARMOR;
    weapon = NOWEAPON;
    PriorityList = { NOOP };
    color = Color::FG_DEFAULT;
    name="Dummy";
}
Dummy::~Dummy(){
    
    //PriorityList.clear();

}
Armor Dummy::getArmor() const{
    return armor;
}
Weapon Dummy::getWeapon() const{
    return weapon;
}
std::vector<Move> Dummy::getPriorityList()const{
    return PriorityList;
}
const std::string Dummy::getFullName() const{
    return "Dummy" + getBoardID();
}
Color::Code Dummy::getColorID() const{
	return color;
}
const std::string Dummy::getName() const{
    return name;
}
