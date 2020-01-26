#include "Board.h"
#include <iostream>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
using namespace std;
Board::Board(uint boardSize, std::vector<Player *> *players):boardSize(boardSize){
     this->players= new std::vector<Player *>();
     for(int i=0;i<(*players).size();i++){
          (this->players)->push_back((*players)[i]);
     }
     storm=0;
}
Board::~Board(){
     /*(*players).clear();
     delete[] players;*/
}

uint Board::getSize() const{
     return boardSize;
}
std::vector<Player *> * Board::getPlayers() const{
     return players;
}
    /**
     * Decide whether the coordinate is in the board limits.
     *
     * @param coord Coordinate to search.
     * @return true if coord is in limits, false otherwise.
     */
bool Board::isCoordInBoard(const Coordinate& coord) const {
     if(coord.x >= boardSize || coord.x < 0 || coord.y >=boardSize || coord.y < 0)  

          return false;
     return true;
}
    /**
     * Decide whether the given coordinate is in storm.
     *
     * @param coord Coordinate to search.
     * @return true if covered in storm, false otherwise.
     */

bool Board::isStormInCoord(const Coordinate &coord) const{
     uint aa=boardSize-1;
     if(!isCoordInBoard(coord)) return false;
     if(isCoordHill(coord)) return false;
     if(coord.x>=int(storm) && coord.x<=aa-int(storm) &&coord.y>=int(storm) && coord.y<=aa-int(storm) ) 
          return false;
     return true;
}

    /**
     * Decide whether the given coordinate is the hill.
     *
     * @param coord Coordinate to search.
     * @return true if the coord is at the very center of the board, false otherwise.
     */
bool Board::isCoordHill(const Coordinate& coord) const{
     uint aa=boardSize/2;
     if(coord.x==aa && coord.y == aa) return true;
     return false;
}
    /**
     * Indexing.
     *
     * Find the player in coordinate.
     *
     * nullptr if player does not exists in given coordinates, or !isCoordInBoard
     *
     * @param coord  The coordinate to search.
     * @return The player in coordinate.
     */
Player* Board::operator[](const Coordinate& coord) const{
     int b=(*players).size();
     for(int i =0;i<b;i++){
         if((*players)[i]->getCoord()==coord) return (*players)[i];
     }
     return nullptr;
}
    /**
     * Calculate the new coordinate with the given move and coordinate.
     *
     * NOOP and ATTACK are no-op, return coord.
     *
     * The new coordinate cannot be outside of the borders.
     * If it's the case, return coord.
     *
     * Also, if there's another player in the new coordinate,
     * return coord.
     *
     * @param move Move to be made.
     * @param coord The coordinate to be moved.
     * @return Calculated coordinate after the move.
     */
Coordinate Board::calculateCoordWithMove(Move move, const Coordinate &coord) const{
     if(isCoordInBoard(coord + move)) return coord+move;
     return coord;
}

    /**
     * Find the visible coordinates from given coordinate.
     *
     * Explanation: The immediate UP/DOWN/LEFT/RIGHT tiles must be calculated.
     *
     * There could be max of 4 tiles, and min of 2 tiles (on corners).
     * Every found coordinate must be in the board limits.
     *
     * If the given coordinate is not in board, return a vector with size = 0. Order does NOT matter.
     *
     * Example:
     *
     * 01----
     * 02HH--
     * ------
     *
     * visibleCoordsFromCoord(Coordinate(0, 0)) == { (1, 0), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(1, 1)) == { (1, 0), (2, 1), (1, 2), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(-1, 0)) == { }
     *
     * @param coord The coordinate to search.
     * @return All coordinates visible.
     */
std::vector<Coordinate> Board::visibleCoordsFromCoord(const Coordinate &coord) const{
     std::vector<Coordinate> v={};
     int xx=coord.x, yy=coord.y;
     if(!isCoordInBoard(coord)) return v;
     if(isCoordInBoard(Coordinate(xx-1,yy))) v.push_back(Coordinate(xx-1,yy));
     if(isCoordInBoard(Coordinate(xx,yy-1))) v.push_back(Coordinate(xx,yy-1));
     if(isCoordInBoard(Coordinate(xx+1,yy))) v.push_back(Coordinate(xx+1,yy));
     if(isCoordInBoard(Coordinate(xx,yy+1))) v.push_back(Coordinate(xx,yy+1));
     return v;
}

    /**
     * Calculate the storm according to the currentRound.
     *
     * @param currentRound The current round being played.
     */
void Board::updateStorm(uint currentRound){
     if (uint(boardSize/2)==storm) return;
     storm=currentRound*0.2-0.2;
}
