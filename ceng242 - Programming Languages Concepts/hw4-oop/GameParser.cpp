#include "GameParser.h"
#include <iostream>
#include <fstream>
#include "Tracer.h"
#include "Berserk.h"
#include "Ambusher.h"
#include "Pacifist.h"
#include "Dummy.h"
#include <string>
#include <iterator>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
using namespace std;

std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string& filename){
    fstream myfile (filename);
    string input,aa;
    vector<Player *> *list= new std::vector<Player *>();
    string delimiter1 = ": ";
    string delimiter2 = "\n";
    string delimiter3 = "::";
    int i =1,board;
    while(getline (myfile,input)){
        if(i==1){
        	aa=input.substr(input.find(delimiter1)+delimiter1.length(), input.find(delimiter2));
            board=std::stoi(aa);  
            aa.clear();          
            i++;
        }
        else if(i==2) i++;
        else{
        	int a,b,c;
	        string name;
	        aa= input.substr(0, input.find(delimiter3));
            a=std::stoi(aa);
			input.erase(0, input.find(delimiter3)+delimiter3.length());
    		name=( input.substr(0, input.find(delimiter3)));
    		input.erase(0, input.find(delimiter3)+delimiter3.length());
    		aa=input.substr(0, input.find(delimiter3));
			b=std::stoi(aa);
			aa.clear();
			input.erase(0, input.find(delimiter3)+delimiter3.length());
			aa= input.substr(0, input.find(delimiter2));
			c=std::stoi(aa);
			aa.clear();
    		if(name=="Berserk\0") {
    			Player * player1 =new Berserk(a,b,c);
    			(*list).push_back(player1);
    			//delete player;
    		}
    		else if(name=="Dummy\0") {
    			Player * player2 =new Dummy(a,b,c);
    			(*list).push_back(player2);
    			//delete player;
    		}
   		    else if(name=="Tracer\0") {
   		    	Player * player3 = new Tracer(a,b,c);
    			(*list).push_back(player3);
    			//delete player;
    		}
    		else if(name=="Ambusher\0") {
    			Player * player4 =new Ambusher(a,b,c);
    			(*list).push_back(player4);
    			//delete player;
    		}
    		else if(name=="Pacifist\0") {
    			Player * player5 =new Pacifist(a,b,c);
    			(*list).push_back(player5);
    			//delete player;
    		}
    		name.clear();
        }
        input.clear();
    }
    delimiter1.clear();
    delimiter2.clear();
    delimiter3.clear();
    pair<int, std::vector<Player *> *> of (board,list);
    return of;
}