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
	cin >> _state;
	_state.setup();
	_mapSystem.setup();
	endTurn();

	//continues making moves while the game is not over
	while (cin >> _state)
	{
		_state.updateVisionInformation();
		makeMoves();
		endTurn();
	}
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
	_state._bug << "turn " << _state._turn << ":" << endl;
	_state._bug << _state << endl;

	for (auto& pair : _orders) {
		delete pair.first;
		delete pair.second;
	}
	_orders.clear();


	// add all locations to unseen tiles set, run once
	if (_unseenTiles.empty()) {
		for (int _row = 0; _row < _state._rows; _row++) {
			for (int _col = 0; _col < _state._cols; _col++) {
				_unseenTiles.insert(new Location(_row, _col));
			}
		}
	}

	for (std::set<Location*>::iterator it = _unseenTiles.begin(); it != _unseenTiles.end();) {
		if (_state._grid[(*it)->_row][(*it)->_col]._isVisible) {
			delete* it;
			it = _unseenTiles.erase(it);
		}
		else {
			++it;
		}
	}

	// prevent stepping on own hill
	for (Location myHill : _state._myHills) {
		Location* mHill = new Location(myHill._row, myHill._col);

		_orders.insert({ mHill,new Location(-1,-1) });
	}

	// Go to close food
	std::map<Location, Location> foodTargets = std::map<Location, Location>();
	std::vector<Route> foodRoutes;
	std::vector<Location> sortedFood = _state._food;
	std::vector<Location> sortedAnts = _state._myAnts;

	// Calculate all routes from ants to food
	for (Location foodLoc : sortedFood) {
		for (Location antLoc : sortedAnts) {
			int manhattanDistance = _state.manhattanDistance(antLoc, foodLoc);
			Route route(antLoc, foodLoc, manhattanDistance);
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
	for (Location enemyHill : _state._enemyHills) {
		if (_enemyHills.count(&enemyHill) == 0) {
			_enemyHills.insert(&enemyHill);
		}
	}

	// Attack hills
	std::vector<Route> hillRoutes;
	for (Location* hillLoc : _enemyHills) {
		for (Location antLoc : sortedAnts) {
			if (!LocationMapContainsValue(_orders, antLoc)) {
				int manhattanDistance = _state.manhattanDistance(antLoc, *hillLoc);
				Route route = Route(antLoc, *hillLoc, manhattanDistance);
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
		if (!LocationMapContainsValue(_orders, antLoc)) {

			std::vector<Route> unseenRoutes;
			for (const Location* unseenLoc : _unseenTiles) {
				int manhattanDistance = _state.manhattanDistance(antLoc, *unseenLoc);
				if (manhattanDistance > 30) continue;
				Route route = Route(antLoc, *unseenLoc, manhattanDistance);
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
	for (Location myHill : _state._myHills) {
		auto it = std::find(_state._myAnts.cbegin(), _state._myAnts.cend(), myHill);
		if (it != _state._myAnts.end() && !LocationMapContainsValue(_orders, { it->_row, it->_col })) {
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

	_state._bug << "time taken: " << _state._timer.getTime() << "ms" << endl << endl;
};

bool Bot::doMoveDirection(const Location& antLoc, int direction) {
	Location loc = _state.getLocation(antLoc, direction);
	// Is there an ant here?
	if (_state._grid[loc._row][loc._col]._isMyAnt)
		return false;

	// Is Location walkable and no ant wants to move at Location?
	if (!_state._grid[loc._row][loc._col]._isWater && !LocationMapContainsKey(_orders, loc))
	{

		_state.makeMove(antLoc, direction);
		Location* newAntLoc = new Location(antLoc._row, antLoc._col);
		Location* newLoc = new Location(loc._row, loc._col);
		_orders.insert({ newLoc, newAntLoc });
		return true;
	}
	else {
		return false;
	}
}


bool Bot::doMoveLocation(const Location& antLoc, const Location& destLoc) {
	std::vector<int> directions = _state.getDirections(antLoc, destLoc);
	for (int direction : directions) {
		if (doMoveDirection(antLoc, direction)) {
			return true;
		}
	}
	return false;
}

bool Bot::doesAnotherAntWantToGoThere(const Location& tile)
{
	for (std::map<Location*, Location*>::iterator it = _orders.begin(); it != _orders.end(); ++it) {
		if ((*it->first)._col == tile._col && (*it->first)._row == tile._row)
		{
			// An ant already wants to go at this Location
			return true;
		}
	}
	return false;
}

bool Bot::LocationMapContainsValue(std::map<Location*, Location*>& locMap, const Location& value) {
	for (std::map<Location*, Location*>::iterator it = locMap.begin(); it != locMap.end(); ++it) {
		if ((*it->second) == value)
		{
			return true;
		}
	}
	return false;
}

bool Bot::LocationMapContainsValue(std::map<Location, Location>& locMap, const Location& value) {
	for (std::map<Location, Location>::iterator it = locMap.begin(); it != locMap.end(); ++it) {
		if (it->second == value)
		{
			return true;
		}
	}
	return false;
}

bool Bot::LocationMapContainsKey(std::map<Location*, Location*>& locMap, const Location& key) {
	for (std::map<Location*, Location*>::iterator it = locMap.begin(); it != locMap.end(); ++it) {
		if ((*it->first) == key)
		{
			return true;
		}
	}
	return false;
}

void Bot::printOrders()
{
	_state._bug << "Tiles the ants want to go to : " << endl;
	for (std::map<Location*, Location*>::iterator it = _orders.begin(); it != _orders.end(); ++it)
		_state._bug << (*it->first)._row << " " << (*it->first)._col << endl;
}

void Bot::printLocationVector(std::vector<Location> locations)
{
	for (const Location& loc : locations) {
		_state._bug << "(" << loc._row << ", " << loc._col << ")" << endl;
	}
}

void Bot::printLocationMap(std::map<Location*, Location*> locations)
{
	for (std::map<Location*, Location*>::iterator it = locations.begin(); it != locations.end(); ++it) {
		_state._bug << "Key : (" << (*it->first)._row << " " << (*it->first)._col << ") | ";
		_state._bug << "Value : (" << (*it->second)._row << " " << (*it->second)._col << ")" << endl;
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
	_state._bug << "Ant at : " << route.getStart()._row << " " << route.getStart()._col << endl;
	_state._bug << "Food at : " << route.getEnd()._row << " " << route.getEnd()._col << endl;
	_state._bug << "Distance is : (" << route.getDistance() << ")" << endl;
	_state._bug << "/////////////////////" << endl;
}

//finishes the turn
void Bot::endTurn()
{
	if (_state._turn > 0)
		_state.reset();
	_state._turn++;

	cout << "go" << endl;
};
