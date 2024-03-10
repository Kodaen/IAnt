#include "GlobalBlackboard.h"

GlobalBlackboard GlobalBlackboard::_instance;

GlobalBlackboard::GlobalBlackboard() {}

bool GlobalBlackboard::doMoveDirection(const Location& antLoc, int direction)
{
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

bool GlobalBlackboard::doMoveLocation(const Location& antLoc, const Location& destLoc)
{
	// TODO : Don't move if Location is not reachable for some reason (shouldn't happen but just in case)
	std::vector<int> directions = _state.getDirections(antLoc, destLoc);
	for (int direction : directions) {
		if (doMoveDirection(antLoc, direction)) {
			return true;
		}
	}
	return false;
}

bool GlobalBlackboard::LocationMapContainsKey(std::map<Location*, Location*>& locMap, const Location& key) {
	for (std::map<Location*, Location*>::iterator it = locMap.begin(); it != locMap.end(); ++it) {
		if ((*it->first) == key)
		{
			return true;
		}
	}
	return false;
}
