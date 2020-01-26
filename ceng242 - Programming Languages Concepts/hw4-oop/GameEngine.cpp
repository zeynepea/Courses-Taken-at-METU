#include "GameEngine.h"
#include "Pacifist.h"
#include "Dummy.h"
#include "Berserk.h"
#include "Tracer.h"
#include "Ambusher.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Entity.h"
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
    /**
     * Constructor.
     *
     * GameEngine "owns" these players.
     * GameEngine also "owns" the vector.
     *
     * @param boardSize The side length of the board.
     * @param players All players to participate in the game.
     */
using namespace std;
GameEngine::GameEngine(uint boardSize, std::vector<Player *> *players):board(boardSize,players){
     currentRound=1;
     flag=0;
}
GameEngine::~GameEngine(){
     
    for(Player *player : *(board.getPlayers())) {
        delete player;
    }

    board.getPlayers()->clear();
    delete board.getPlayers();
}

const Board& GameEngine::getBoard() const{
     return board;
}

    /**
     * Indexing.
     *
     * Find the player with given ID.
     *
     * nullptr if not exists.
     *
     * @param id ID of the player.
     * @return The player with given ID.
     */
Player* GameEngine::operator[](uint id) const{
     vector<Player *> *xx= board.getPlayers();
     for(int i =0;i<(*xx).size();i++){
          if((*xx)[i]->getID()==id) {
              return (*xx)[i];
          }
     }
     return nullptr;
}

    /**
     * Decide whether the game is finished.
     *
     * @return true if there is only 1 player (alive), on top of the hill; or there are 0 players. False otherwise.
     */
bool GameEngine::isFinished() const{
     vector<Player *> *xx= board.getPlayers();
     Coordinate a=(*xx)[0]->getCoord();
     if((*xx).size()==0) {
          return true;
     }
     if((*xx).size()==1 && board.isCoordHill(a) ) {
          return true;
     }
     return false;
}

    /**
     * Take turn for every player.
     *
     * How-to:
     * - Announce turn start (cout).
     * Example: -- START ROUND 1  --
     * - board.updateStorm(currentRound)
     * - For every player (sorted according to their IDs) that isn't dead (HP <= 0):
     *      - takeTurnForPlayer(player).
     * - Announce turn end (cout).
     * Example: -- END ROUND 1 --
     */
void GameEngine::takeTurn(){
     cout<<"-- START ROUND"<<currentRound<<" --"<<endl;
     board.updateStorm(currentRound);
     vector<Player *> *xx= board.getPlayers();
     if((*xx).size()==2){
          if((*xx)[0]->getID() > (*xx)[1]->getID() )
               reverse((*xx).begin(),(*xx).end());
     }
     //else sort((*xx).begin(), (*xx).end(),less<Player *>());
     for(int i=0;i<(*xx).size();i++){
          takeTurnForPlayer((*xx)[i]->getID());
          if(flag){
               --i;
               flag=0;
          }
     }
     cout<<"-- END ROUND"<<currentRound<<" --"<<endl;
     ++currentRound;
}

    /**
     * The most important (algorithm-wise) method.
     *
     * How-to:
     * - Get player with ID. Return NOOP if not exists.
     * - Get player's priority list.
     * - Get player's visibility from the board (visibleCoordsFromCoord).
     *
     * - If the player is in the storm (isStormInCoord), announce the damage and give player stormDamage.
     * - Example: Tracer01(10) is STORMED! (-10)
     *
     * - If dead, announce the death, remove player from the board/list/anywhere, and return NOOP.
     * - Example: Tracer01(0) DIED.
     *
     * - For MOVE in player's priority list:
     *          - If the MOVE is NOOP:
     *              - return NOOP.
     *          - Else if the MOVE is ATTACK:
     *              - Get all players that this player can attack (board[coord] for each in visibilityCoords).
     *              - If none, continue.
     *              - Else:
     *                  - Pick the one with most priority (lowest ID).
     *                  - isPlayerDead = player.attackTo(thatPlayer).
     *                  - if isPlayerIsDead:
     *                      - announce the death.
     *                      - remove thatPlayer from the board/list/anywhere.
     *                  - return ATTACK.
     *          - Else (UP/DOWN/LEFT/RIGHT):
     *              - calculateCoordWithMove(move).
     *              - If the new coordinate is different than the player's (meaning it's able to do that move)
     *              AND the player is getting closer to the hill;
     *                  - player.executeMove(MOVE).
     *                  - return MOVE.
     *              - Else:
     *                  - continue.
     *
     * // If the priority list is exhausted;
     * return NOOP.
     *
     * @param player Player ID to move.
     * @return move Decided move.
     */
Move GameEngine::takeTurnForPlayer(uint playerID){
     vector<Player *> *xx= board.getPlayers();
     int i;
     for(i=0;i<(*xx).size();i++){
         if((*xx)[i]->getID()==playerID){
              break;
         }
     }
     int a=i;
     if(a==(*xx).size()) {
        xx=nullptr;
        return NOOP;
     }
     if(board.isStormInCoord((*xx)[a]->getCoord())) {
          cout<<(*xx)[a]->getFullName()<<"("<<(*xx)[a]->getHP()<<") is STORMED! (-"<<Entity::stormDamageForRound(currentRound)<<")"<<endl;
          (*xx)[a]->updateHP(-Entity::stormDamageForRound(currentRound));
          if((*xx)[a]->getHP()<=0){ 
               cout<<(*xx)[a]->getFullName()<<"("<<(*xx)[a]->getHP()<<") DIED."<<endl;
               (*xx).erase((*xx).begin()+a);
               flag=1;
               xx=nullptr;
               return NOOP;
          }
     }
     for(int j=0;j<(*xx)[a]->getPriorityList().size();j++){
          if(((*xx)[a]->getPriorityList())[j]==NOOP) {
               xx=nullptr;
               return NOOP;
          }
          else if(((*xx)[a]->getPriorityList())[j]==ATTACK){
               std::vector<Coordinate> co=board.visibleCoordsFromCoord((*xx)[a]->getCoord());
               uint idd=200;
               int nerede=0;
               for (int i=0;i<co.size();i++){
                    if(board[co[i]]!=nullptr) 
                        if(board[co[i]]->getID()<idd) {
                          nerede=i;
                          idd=board[co[i]]->getID();
                        }
               }
               if(idd==200) continue;
               if((*xx)[a]->attackTo(board[co[nerede]])){
                    cout<<board[co[nerede]]->getFullName()<<"("<<board[co[nerede]]->getHP()<<") DIED."<<endl;
                    int s;
                    for(s=0;s<(*xx).size();s++){
                         if((*xx)[s]->getID()==board[co[nerede]]->getID()){
                             if((*xx)[s]->getID()<(*xx)[a]->getID()) flag=1;
                             break;
                         }
                    }
                    (*xx).erase((*xx).begin()+s);
               }
               co.clear();
               xx=nullptr;
               return ATTACK;
          }
          else{
               Coordinate of=(*xx)[a]->getCoord();
               of=of+((*xx)[a]->getPriorityList())[j];
               if(board.isCoordInBoard(of) && board[of]==nullptr) {
                    Coordinate hill(board.getSize()/2,board.getSize()/2);
                    if(((*xx)[a]->getCoord()-hill)>(of-hill)){
                        (*xx)[a]->executeMove(((*xx)[a]->getPriorityList())[j]);                        
                        Move e =((*xx)[a]->getPriorityList())[j];
                        xx=nullptr;
                        return e;
                    }
                    else continue;
               }
               else continue;
          }
     }
     xx=nullptr;
     return NOOP;

     
}

    /**
     * Find the winner player.
     *
     * nullptr if there are 0 players left, or the game isn't finished yet.
     *
     * @return The winner player.
     */
Player * GameEngine::getWinnerPlayer() const{
     if(isFinished()) {
         vector<Player *> *xx= board.getPlayers();
         if((*xx).size()==1) return (*xx)[0];
         else return nullptr;
     }
     else return nullptr;
} 