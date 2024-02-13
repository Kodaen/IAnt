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

	// add all locations to unseen tiles set, run once
	if (unseenTiles->empty()) {
		for (int row = 0; row < state.rows; row++) {
			for (int col = 0; col < state.cols; col++) {
				unseenTiles->insert(Location(row, col));
			}
		}
	}

	for (std::set<Location>::iterator it = unseenTiles->begin(); it != unseenTiles->end(); ++it) {
		if (state.grid[it->row][it->col].isVisible) {
			it = unseenTiles->erase(it);
		}
	}

	// prevent stepping on own hill
	for (Location myHill : state.myHills) {
		orders->insert({ myHill, Location(-1,-1) });
	}

	// Go to close food
	std::map<Location, Location> foodTargets = std::map<Location, Location>();
	std::vector<Route> foodRoutes;
	std::vector<Location> sortedFood = state.food;
	std::vector<Location> sortedAnts = state.myAnts;

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
	for (Location myHill : state.myHills) {
		auto it = std::find(state.myAnts.cbegin(), state.myAnts.cend(), myHill);
		if (it != state.myAnts.end() && !LocationMapContainsValue(*orders, {it->row, it->col})) {
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

	// Is there an ant here?
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
