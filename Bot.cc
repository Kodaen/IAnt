#include <set>

#include "Bot.h"
#include "BehaviorTree.h"
#include "NearbyFoodAnts.h"

using namespace std;

//constructor
Bot::Bot()
{
};

//plays a single game of Ants.
void Bot::playGame()
{
	//reads the game parameters and sets up
	cin >> r_gbb._state;
	r_gbb._state.setup();
	MapSystem::getInstance()->setup();
	endTurn();

	//continues making moves while the game is not over
	while (cin >> r_gbb._state)
	{
		r_gbb._state.updateVisionInformation();
		makeMoves();
		endTurn();
	}
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
	r_gbb._state._bug << "turn " << r_gbb._state._turn << ":" << endl;
	r_gbb._state._bug << r_gbb._state << endl;

	BehaviorTree* bt = new BehaviorTree();

	for (auto& pair : r_gbb._orders) {
		delete pair.first;
		delete pair.second;
	}
	r_gbb._orders.clear();
	r_gbb._nearbyFoodAnts.clear();

	associateFoodToNearbyAnts();

	bt->selector()
			.sequencer() //	[EAT]
				.input(INPUT_CLOSE_ANY_FOOD)
				.input(INPUT_CLOSEST_TO_FOOD)
				.action(ACTION_CALC_TRAJ_FOR_FOOD)
				.decorator(DECORATOR_NOT)
					.input(INPUT_I_DIE_BY_GOING_THERE)
					.selectParent()
				.action(ACTION_APPROACH_FOOD)
				.action(ACTION_BLACKBOARD_INFOS)
				;

	for (Location& ant : r_gbb._state._myAnts)
	{
		bt->execute(ant);
	}


	// add all locations to unseen tiles set, run once
	if (_unseenTiles.empty()) {
		for (int _row = 0; _row < r_gbb._state._rows; _row++) {
			for (int _col = 0; _col < r_gbb._state._cols; _col++) {
				_unseenTiles.insert(new Location(_row, _col));
			}
		}
	}

	for (std::set<Location*>::iterator it = _unseenTiles.begin(); it != _unseenTiles.end();) {
		if (r_gbb._state._grid[(*it)->_row][(*it)->_col]._isVisible) {
			delete* it;
			it = _unseenTiles.erase(it);
		}
		else {
			++it;
		}
	}

	// prevent stepping on own hill
	for (Location myHill : r_gbb._state._myHills) {
		Location* mHill = new Location(myHill._row, myHill._col);

		r_gbb._orders.insert({ mHill,new Location(-1,-1) });
	}

	// Go to close food
	std::map<Location, Location> foodTargets = std::map<Location, Location>();
	std::vector<Route> foodRoutes;
	std::vector<Location> sortedFood = r_gbb._state._food;
	std::vector<Location> sortedAnts = r_gbb._state._myAnts;

	//// Calculate all routes from ants to food
	//for (Location foodLoc : sortedFood) {
	//	for (Location antLoc : sortedAnts) {
	//		int manhattanDistance = r_gbb._state.manhattanDistance(antLoc, foodLoc);
	//		Route route(antLoc, foodLoc, manhattanDistance);
	//		foodRoutes.push_back(route);
	//	}
	//}

	//// Sort routes from ants to food from the shortest to the longest
	//std::sort(foodRoutes.begin(), foodRoutes.end());
	//// We send the closest ants first, some ants my not move because there is no food left
	//for (Route& route : foodRoutes) {
	//	if (foodTargets.count(route.getEnd()) == 0
	//		&& !LocationMapContainsValue(foodTargets, route.getStart())
	//		&& doMoveLocation(route.getStart(), route.getEnd())) {
	//		foodTargets[route.getEnd()] = route.getStart();
	//	}
	//}

	// Add new hills to set
	for (Location enemyHill : r_gbb._state._enemyHills) {
		if (_enemyHills.count(&enemyHill) == 0) {
			_enemyHills.insert(&enemyHill);
		}
	}

	// Attack hills
	std::vector<Route> hillRoutes;
	for (Location* hillLoc : _enemyHills) {
		for (Location antLoc : sortedAnts) {
			if (!LocationMapContainsValue(r_gbb._orders, antLoc)) {
				int manhattanDistance = r_gbb._state.manhattanDistance(antLoc, *hillLoc);
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
		if (!LocationMapContainsValue(r_gbb._orders, antLoc)) {

			std::vector<Route> unseenRoutes;
			for (const Location* unseenLoc : _unseenTiles) {
				int manhattanDistance = r_gbb._state.manhattanDistance(antLoc, *unseenLoc);
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
	for (Location myHill : r_gbb._state._myHills) {
		auto it = std::find(r_gbb._state._myAnts.cbegin(), r_gbb._state._myAnts.cend(), myHill);
		if (it != r_gbb._state._myAnts.end() && !LocationMapContainsValue(r_gbb._orders, { it->_row, it->_col })) {
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

	r_gbb._state._bug << "time taken: " << r_gbb._state._timer.getTime() << "ms" << endl << endl;
};

bool Bot::doMoveDirection(const Location& antLoc, int direction) {
	Location loc = r_gbb._state.getLocation(antLoc, direction);
	// Is there an ant here?
	if (r_gbb._state._grid[loc._row][loc._col]._isMyAnt)
		return false;

	// Is Location walkable and no ant wants to move at Location?
	if (!r_gbb._state._grid[loc._row][loc._col]._isWater && !LocationMapContainsKey(r_gbb._orders, loc))
	{

		r_gbb._state.makeMove(antLoc, direction);
		Location* newAntLoc = new Location(antLoc._row, antLoc._col);
		Location* newLoc = new Location(loc._row, loc._col);
		r_gbb._orders.insert({ newLoc, newAntLoc });
		return true;
	}
	else {
		return false;
	}
}


bool Bot::doMoveLocation(const Location& antLoc, const Location& destLoc) {
	std::vector<int> directions = r_gbb._state.getDirections(antLoc, destLoc);
	for (int direction : directions) {
		if (doMoveDirection(antLoc, direction)) {
			return true;
		}
	}
	return false;
}

// TODO : Doesn't work properly when the closest ant cannot go directly to food due to obstacle
void Bot::associateFoodToNearbyAnts() {
	for (Location& food : r_gbb._state._food) {
		// r_gbb._state._bug << "	Food (" << food._row << ", " << food._col << ")" << endl;
		//r_gbb._state._bug << "Nearby ants :" << endl;

		int viewRadius = (int)std::floor(r_gbb._state._viewRadius);
		NearbyFoodAnts NFA = {food, std::set<Location>(), Location(), 9999};

		// We search nearby ants in a square of length 16x16
		for (int col = -viewRadius; col < viewRadius; col++)
		{
			for (int row = -viewRadius; row < viewRadius; row++)
			{
				auto gridRow = (r_gbb._state._rows + (food._row + row)) % r_gbb._state._rows;
				auto gridCol = (r_gbb._state._cols + (food._col + col)) % r_gbb._state._cols;

				// If there is an ant nearby the food at this location
				// put it in the NFA
				if (r_gbb._state._grid[gridRow][gridCol]._isMyAnt) {
					Location nearbyAntLoc = { gridRow,gridCol };
					NFA.nearbyAntsLoc.insert(nearbyAntLoc);

					// If this ant is the closest to the food, update closestAntLoc
					float distanceToFood = MapSystem::getInstance()->getManhattanDistance(food, nearbyAntLoc);
					if (distanceToFood < NFA.closestDistance)
					{
						NFA.closestAntLoc = nearbyAntLoc;
						NFA.closestDistance = distanceToFood;
					}
				}
			}
		}

		if (NFA.nearbyAntsLoc.size() > 0)
		{
			r_gbb._nearbyFoodAnts.push_back(NFA);
		}

	}
}

bool Bot::doesAnotherAntWantToGoThere(const Location& tile)
{
	for (std::map<Location*, Location*>::iterator it = r_gbb._orders.begin(); it != r_gbb._orders.end(); ++it) {
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
	r_gbb._state._bug << "Tiles the ants want to go to : " << endl;
	for (std::map<Location*, Location*>::iterator it = r_gbb._orders.begin(); it != r_gbb._orders.end(); ++it)
		r_gbb._state._bug << (*it->first)._row << " " << (*it->first)._col << endl;
}

void Bot::printLocationVector(std::vector<Location> locations)
{
	for (const Location& loc : locations) {
		r_gbb._state._bug << "(" << loc._row << ", " << loc._col << ")" << endl;
	}
}

void Bot::printLocationMap(std::map<Location*, Location*> locations)
{
	for (std::map<Location*, Location*>::iterator it = locations.begin(); it != locations.end(); ++it) {
		r_gbb._state._bug << "Key : (" << (*it->first)._row << " " << (*it->first)._col << ") | ";
		r_gbb._state._bug << "Value : (" << (*it->second)._row << " " << (*it->second)._col << ")" << endl;
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
	r_gbb._state._bug << "Ant at : " << route.getStart()._row << " " << route.getStart()._col << endl;
	r_gbb._state._bug << "Food at : " << route.getEnd()._row << " " << route.getEnd()._col << endl;
	r_gbb._state._bug << "Distance is : (" << route.getDistance() << ")" << endl;
	r_gbb._state._bug << "/////////////////////" << endl;
}

//finishes the turn
void Bot::endTurn()
{
	if (r_gbb._state._turn > 0)
		r_gbb._state.reset();
	r_gbb._state._turn++;

	cout << "go" << endl;
};
