#ifndef BOT_H_
#define BOT_H_

#include <map>
#include <set>

#include "State.h"
#include "Location.h"
#include "Route.h"
#include "MapSystem.h"
#include "GlobalBlackboard.h"

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();

    std::set<Location*> _unseenTiles;
    std::set<Location> _enemyHills;


    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    bool doMoveDirection(const Location &antLoc, int direction); // move to tile if tile is walkable and another ant doesn't already want to walk here
    bool doMoveLocation(const Location& antLoc, const Location& destLoc);

    void endTurn();     //indicates to the engine that it has made its moves

    void associateFoodToNearbyAnts();

    // ------------ UTILS ------------ //
    bool doesAnotherAntWantToGoThere(const Location &tile);    // Check if another ant wants to walk on given tile
    inline bool LocationMapContainsValue(std::map<Location*, Location*> &locMap,const Location &value);
    inline bool LocationMapContainsValue(std::map<Location, Location> &locMap, const Location &value);
    inline bool LocationMapContainsKey(std::map<Location*, Location*> &locMap, const Location &key);

    // ------------ DEBUG ------------ //
    void printOrders();
    void printLocationVector(std::vector<Location> loc);
    void printLocationMap(std::map<Location*, Location*> locations);
    void printRouteVector(std::vector<Route> routes);
    void printRoute(Route routes);
    double totalTime = 0;
};

#endif //BOT_H_
