#ifndef BOT_H_
#define BOT_H_

#include <map>

#include "State.h"
#include "Location.h"
#include "Route.h"

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
    bool doMoveDirection(const Location &antLoc, int direction); // move to tile if tile is walkable and another ant doesn't already want to walk here
    bool doMoveLocation(Location antLoc, Location destLoc);

    void endTurn();     //indicates to the engine that it has made its moves

    // ------------ UTILS ------------ //
    bool doesAnotherAndWantToGoThere(Location tile);    // Check if another ant wants to walk on given tile
    bool Bot::LocationMapContainsKey(std::map<Location, Location> locMap, Location key);
    bool LocationMapContainsValue(std::map<Location, Location> locMap, Location value);

    // ------------ DEBUG ------------ //
    void printOrders();
    void printLocationVector(std::vector<Location> loc);
    void printLocationMap(std::map<Location, Location> locations);
    void printRouteVector(std::vector<Route> routes);
    void printRoute(Route routes);
};

#endif //BOT_H_
