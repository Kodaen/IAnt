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

	bt->selector()		// (ANT)
			.sequencer()	// [EAT]
				.input(INPUT_CLOSEST_TO_FOOD)
				.action(ACTION_GET_CLOSEST_FOOD)
				.action(ACTION_CALC_TRAJ_FOR_FOOD)
				.decorator(DECORATOR_NOT)
					.input(INPUT_I_DIE_BY_GOING_THERE)
					.selectParent()
				.action(ACTION_APPROACH_FOOD)
				.selectParent()
			.sequencer()	// [SOLO ATTACK]
				.input(INPUT_ENEMY_NEAR)
				.sequencer()		// [FIGHT ENEMY]
					.sequencer()		// [SOLO COMBAT]
						.input(INPUT_WE_BOTH_DIE)
						.input(INPUT_CLOSEST_TO_MY_HILL)
						.action(ACTION_APPROACH_ENEMY)
						.selectParent()
				//.action(ACTION_CALL_BACKUP)
					.selectParent()
				.selectParent()
			.sequencer()	// [EXPLORE]
				.action(ACTION_CHOOSE_SENTINEL_POINT)
				.action(ACTION_APPROACH_SENTINEL_POINT)
				.selectParent()
			.action(ACTION_BLACKBOARD_INFOS)
				;

	for (Location& ant : r_gbb._state._myAnts)
	{
		MapSystem::getInstance()->updateSentinelsPoint(ant,r_gbb._state._turn);
		bt->execute(ant);
	}

	r_gbb.emptyExplorationCache();
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
		 //r_gbb._state._bug << "	Food (" << food._row << ", " << food._col << ")" << endl;
		//r_gbb._state._bug << "Nearby ants :" << endl;

		std::vector<Location> closestAnts = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._myAnts, food, 15, 1);
		if (closestAnts.size() == 0) continue;

		NearbyFoodAnts NFA = {food, closestAnts[0]};

		r_gbb._nearbyFoodAnts.push_back(NFA);

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
