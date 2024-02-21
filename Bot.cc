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
	state._bug << "turn " << state._turn << ":" << endl;
	state._bug << state << endl;

	orders->clear();

	// add all locations to unseen tiles set, run once
	if (unseenTiles->empty()) {
		for (int row = 0; row < state._rows; row++) {
			for (int col = 0; col < state._cols; col++) {
				unseenTiles->insert(Location(row, col));
			}
		}
	}

	for (std::set<Location>::iterator it = unseenTiles->begin(); it != unseenTiles->end(); ++it) {
		if (state._grid[it->row][it->col].isVisible) {
			it = unseenTiles->erase(it);
		}
	}

	// prevent stepping on own hill
	for (Location myHill : state._myHills) {
		orders->insert({ myHill, Location(-1,-1) });
	}

	// Go to close food
	std::map<Location, Location> foodTargets = std::map<Location, Location>();
	std::vector<Route> foodRoutes;
	std::vector<Location> sortedFood = state._food;
	std::vector<Location> sortedAnts = state._myAnts;

	// Calculate all routes from ants to food
	for (Location foodLoc : sortedFood) {
		for (Location antLoc : sortedAnts) {
			int distance = state.distance(antLoc, foodLoc);
			Route route(antLoc, foodLoc, distance);
			foodRoutes.push_back(route);
		}
	}

	// Sort routes from ants to food from the shortest to the longest
	std::sort(foodRoutes.begin(), foodRoutes.end());
	// We send the closest ants first, some ants my not move because there is no food left
	for (Route& route : foodRoutes) {
		if (foodTargets.count(route.getEnd()) == 0
			&& !LocationMapContainsValue(foodTargets, route.getStart())
			&& doMoveLocation(route.getStart(), route.getEnd())) {
			foodTargets[route.getEnd()] = route.getStart();
		}
	}

	// Add new hills to set
	for (Location enemyHill : state._enemyHills) {
		if (!_enemyHills.count(enemyHill)) {
			_enemyHills.insert(enemyHill);
		}
	}

	// Attack hills
	std::vector<Route> hillRoutes;
	for (Location hillLoc : _enemyHills) {
		for (Location antLoc : sortedAnts) {
			if (!LocationMapContainsValue(*orders, antLoc)) {
				int distance = state.distance(antLoc, hillLoc);
				Route route = Route(antLoc, hillLoc, distance);
				hillRoutes.push_back(route);
			}
		}
	}
	std::sort(hillRoutes.begin(), hillRoutes.end());
	for (Route route : hillRoutes) {
		doMoveLocation(route.getStart(), route.getEnd());
	}

	// Explore unseen areas
	for (Location antLoc : sortedAnts) {
		if (!LocationMapContainsValue(*orders, antLoc)) {

			std::vector<Route> unseenRoutes;
			for (const Location& unseenLoc : *unseenTiles) {
				int distance = state.distance(antLoc, unseenLoc);
				Route route = Route(antLoc, unseenLoc, distance);
				unseenRoutes.push_back(route);
			}
			std::sort(unseenRoutes.begin(), unseenRoutes.end());
			for (Route route : unseenRoutes) {
				if (doMoveLocation(route.getStart(), route.getEnd())) {
					break;
				}
			}
		}
	}

	// Unblock hills
	for (Location myHill : state._myHills) {
		auto it = std::find(state._myAnts.cbegin(), state._myAnts.cend(), myHill);
		if (it != state._myAnts.end() && !LocationMapContainsValue(*orders, {it->row, it->col})) {
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

	state._bug << "time taken: " << state._timer.getTime() << "ms" << endl << endl;
};

bool Bot::doMoveDirection(const Location& antLoc, int direction) {
	Location newLoc = state.getLocation(antLoc, direction);

	// Is there an ant here?
	if (state._grid[newLoc.row][newLoc.col].isMyAnt)
		return false;

	// Is Location walkable and no ant wants to move at Location?
	if (!state._grid[newLoc.row][newLoc.col].isWater && orders->count(newLoc) == 0)
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

bool Bot::doesAnotherAntWantToGoThere(Location tile)
{
	for (std::map<Location, Location>::iterator it = orders->begin(); it != orders->end(); ++it) {
		if (it->first.col == tile.col && it->first.row == tile.row)
		{
			// An ant already wants to go at this Location
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
	state._bug << "Tiles the ants want to go to : " << endl;
	for (std::map<Location, Location>::iterator it = orders->begin(); it != orders->end(); ++it)
		state._bug << it->first.col << " " << it->first.row << endl;
}

void Bot::printLocationVector(std::vector<Location> locations)
{
	for (const Location& loc : locations) {
		state._bug << "(" << loc.row << ", " << loc.col << ")" << endl;
	}
}

void Bot::printLocationMap(std::map<Location, Location> locations)
{
	for (std::map<Location, Location>::iterator it = locations.begin(); it != locations.end(); ++it) {
		state._bug << "Key : (" << it->first.row << " " << it->first.col << ") | ";
		state._bug << "Value : (" << it->second.row << " " << it->second.col << ")" << endl;
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
	state._bug << "Ant at : " << route.getStart().row << " " << route.getStart().col << endl;
	state._bug << "Food at : " << route.getEnd().row << " " << route.getEnd().col << endl;
	state._bug << "Distance is : (" << route.getDistance() << ")" << endl;
	state._bug << "/////////////////////" << endl;
}

//finishes the turn
void Bot::endTurn()
{
	if (state._turn > 0)
		state.reset();
	state._turn++;

	cout << "go" << endl;
};
