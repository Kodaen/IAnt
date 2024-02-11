#include <set>

#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
	//reads the game parameters and sets up
	cin >> state;
	state.setup();
	endTurn();

	//continues making moves while the game is not over
	while (cin >> state)
	{
		state.updateVisionInformation();
		makeMoves();
		endTurn();
	}
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
	state.bug << "turn " << state.turn << ":" << endl;
	state.bug << state << endl;

	orders->clear();

	// prevent stepping on own hill
	for (Location myHill : state.myHills) {
		orders->insert({ myHill, Location(-1,-1) });

	}

	std::map<Location, Location> foodTargets = std::map<Location, Location>();
	std::vector<Route> foodRoutes;
	std::vector<Location> sortedFood = state.food;
	std::vector<Location> sortedAnts = state.myAnts;

	for (Location foodLoc : sortedFood) {
		for (Location antLoc : sortedAnts) {
			int distance = state.distance(antLoc, foodLoc);
			Route route(antLoc, foodLoc, distance);
			foodRoutes.push_back(route);
		}
	}

	std::sort(foodRoutes.begin(), foodRoutes.end());
	for (Route& route : foodRoutes) {
		if (foodTargets.count(route.getEnd()) == 0
			&& !LocationMapContainsValue(foodTargets, route.getStart())
			&& doMoveLocation(route.getStart(), route.getEnd())) {
			foodTargets[route.getEnd()] = route.getStart();
		}
	}

	// Unblock hills
	for (Location myHill : state.myHills) {
		auto it = std::find(state.myAnts.cbegin(), state.myAnts.cend(), myHill);
		if (it != state.myAnts.end() && !LocationMapContainsValue(*orders, {it->row, it->col})) {
			state.bug << "ANT ON HILL FFS" << endl;

			// If a ant blocks hill, move it if possible
			for (int d = 0; d < TDIRECTIONS; d++)
			{
				if (doMoveDirection(myHill, d))
				{
					break;
				}
			}
		}
	}

	state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

bool Bot::doMoveDirection(const Location& antLoc, int direction) {
	Location newLoc = state.getLocation(antLoc, direction);

	// Is there an ant here? And does it plan on moving?
	if (state.grid[newLoc.row][newLoc.col].isMyAnt)
		return false;

	// Is Location walkable and no ant wants to move at Location?
	if (!state.grid[newLoc.row][newLoc.col].isWater && orders->count(newLoc) == 0)
	{
		state.makeMove(antLoc, direction);
		orders->insert({ newLoc, antLoc });
		return true;
	}
	else {
		return false;
	}
}


bool Bot::doMoveLocation(Location antLoc, Location destLoc) {
	std::vector<int> directions = state.getDirections(antLoc, destLoc);
	for (int direction : directions) {
		if (doMoveDirection(antLoc, direction)) {
			return true;
		}
	}
	return false;
}

bool Bot::doesAnotherAndWantToGoThere(Location tile)
{
	for (std::map<Location, Location>::iterator it = orders->begin(); it != orders->end(); ++it) {
		if (it->first.col == tile.col && it->first.row == tile.row)
		{
			state.bug << "Tile is already visited !" << endl;
			return true;
		}
	}
	return false;
}

bool Bot::LocationMapContainsValue(std::map<Location, Location> locMap, Location value) {
	for (std::map<Location, Location>::iterator it = locMap.begin(); it != locMap.end(); ++it) {
		if (it->second == value)
		{
			return true;
		}
	}
	return false;
}

void Bot::printOrders()
{
	state.bug << "Tiles the ants want to go to : " << endl;
	for (std::map<Location, Location>::iterator it = orders->begin(); it != orders->end(); ++it)
		state.bug << it->first.col << " " << it->first.row << endl;
}

void Bot::printLocationVector(std::vector<Location> locations)
{
	for (const Location& loc : locations) {
		state.bug << "(" << loc.row << ", " << loc.col << ")" << endl;
	}
}

void Bot::printLocationMap(std::map<Location, Location> locations)
{
	for (std::map<Location, Location>::iterator it = locations.begin(); it != locations.end(); ++it) {
		state.bug << "Key : (" << it->first.row << " " << it->first.col << ") | ";
		state.bug << "Value : (" << it->second.row << " " << it->second.col << ")" << endl;
	}

}

void Bot::printRouteVector(std::vector<Route> routes)
{
	for (Route& route : routes) {
		printRoute(route);
	}
}

void Bot::printRoute(Route route)
{
	state.bug << "Ant at : " << route.getStart().row << " " << route.getStart().col << endl;
	state.bug << "Food at : " << route.getEnd().row << " " << route.getEnd().col << endl;
	state.bug << "Distance is : (" << route.getDistance() << ")" << endl;
	state.bug << "/////////////////////" << endl;
}

//finishes the turn
void Bot::endTurn()
{
	if (state.turn > 0)
		state.reset();
	state.turn++;

	cout << "go" << endl;
};
