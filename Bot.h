#ifndef BOT_H_
#define BOT_H_

#include "State.h"
#include <map>
#include "Location.h"

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;
    std::map<Location, Location>* orders = new std::map<Location, Location>();

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    boolean doMoveDirection(const Location &antLoc, int direction); // move to tile if tile is walkable and another ant doesn't already want to walk here
    void endTurn();     //indicates to the engine that it has made its moves

    bool doesAnotherAndWantToGoThere(Location tile);    // Check if another ant wants to walk on given tile
    void printOrders(); // Debug function that prints where all ants want to go 
};

#endif //BOT_H_
