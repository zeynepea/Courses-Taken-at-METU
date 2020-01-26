#include "Berserk.h"
#include <string>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Berserk::Berserk(uint id, int x, int y):Player(id,x,y) {
	HP = 100;
	armor = WOODEN;
	weapon = PISTOL;
	PriorityList = { ATTACK, UP, LEFT, DOWN, RIGHT };
	color = Color::FG_RED;
    name="Berserk";
}
Berserk::~Berserk(){
    
   // PriorityList.clear();

}
Armor Berserk::getArmor() const{
    return armor;
}
Weapon Berserk::getWeapon() const{
    return weapon;
}
std::vector<Move> Berserk::getPriorityList()const{
    return PriorityList;
}
const std::string Berserk::getFullName() const{

    return "Berserk" + getBoardID();
}
const std::string Berserk::getName() const{
    return name;
}
Color::Code Berserk::getColorID() const{
    return color;
}