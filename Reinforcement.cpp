#include "Reinforcement.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

Reinforcement::Reinforcement(Location callingAntPos, Location enemyPos, int helpRadius)
	: _callingAntPos(callingAntPos), _enemyPos(enemyPos), _helpRadius(helpRadius), _isValid(true), _antPositionAlreadyChecked(false), _allAntsInPosition(false)
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	double atkRadius = r_gbb._state._attackRadius;
	int maxAntGroup = 0;
	for (int i = 1; i <= atkRadius; i++)
	{
		maxAntGroup += 4 * i;
	}

	_otherEnemiesPos = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._enemyAnts, _enemyPos, atkRadius, maxAntGroup);
	_otherEnemiesPos.push_back(enemyPos);
}

Reinforcement::Reinforcement(Location callingAntPos, Location enemyPos)
	: Reinforcement(callingAntPos, enemyPos, 8) { }

Reinforcement::Reinforcement() : _isValid(false), _antPositionAlreadyChecked(false), _allAntsInPosition(false)
{ }

bool Reinforcement::tryAskingHelp()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	// Get all ally ants nearby
	std::vector<Location> closeAllyAnts = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._myAnts, _callingAntPos, _helpRadius, _otherEnemiesPos.size() + 10);

	// Remove ants which already acted 
	for (int i = closeAllyAnts.size() - 1; i >= 0; i--)
	{
		if (r_gbb.LocationMapContainsKey(r_gbb._orders, closeAllyAnts[i]))
		{
			closeAllyAnts.erase(closeAllyAnts.begin() + i);
		}
	}

	// Check if we got enough allies to help for the fight
	if (closeAllyAnts.size() < _otherEnemiesPos.size()) {
		// TODO : remove debug log
		//r_gbb._state._bug << "Not enough allies nearby to join the fight : " << _enemyPos << std::endl;
		cancel();
		return false;
	}

	// Put the right number of ally ants in the vector.
	_helpingAntPos = { closeAllyAnts.begin(), closeAllyAnts.begin() + _otherEnemiesPos.size() };
	_helpingAntPos.push_back(_callingAntPos);

	for each (Location helpingAnt in _helpingAntPos)
	{
		// TODO : remove debug log
		// r_gbb._state._bug << helpingAnt << " can help for this fight : " << _enemyPos << " where there is : " << _otherEnemiesPos.size() << std::endl;
	}

	return true;
}

bool Reinforcement::trySetupAtkPos()
{
	_atkDirection = calculateDirection(_enemyPos, false);
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();

	Location closestAntInDirection = _enemyPos;
	Location mostExtremityAnt = _enemyPos;

	// Select the closest ant in the direction the enemy are
	switch (_atkDirection)
	{
	case 0: /*NORTH*/
		// Ant who called for help
		_directionVec = Location(-1, 0);
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = closestAntInDirection;
			reEvaluateLocations(loc1, loc2);

			if (loc1._row > loc2._row) {
				closestAntInDirection = otherEnemyPos;
			}
		}

		break;
	case 1: /*EAST*/
		_directionVec = Location(0, 1);
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = closestAntInDirection;
			reEvaluateLocations(loc1, loc2);

			if (loc1._col < loc2._col) {
				closestAntInDirection = otherEnemyPos;
			}
		}

		break;
	case 2: /*SOUTH*/
		_directionVec = Location(1, 0);
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = closestAntInDirection;
			reEvaluateLocations(loc1, loc2);

			if (loc1._row < loc2._row) {
				closestAntInDirection = otherEnemyPos;
			}
		}

		break;
	case 3: /*WEST*/
		_directionVec = Location(0, -1);
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = closestAntInDirection;
			reEvaluateLocations(loc1, loc2);

			if (loc1._col > loc2._col) {
				closestAntInDirection = otherEnemyPos;
			}
		}
		break;
	default:
		r_gbb._state._bug << "Error, couldn't determine where Enemy : " << _enemyPos << " are located from our ants" << std::endl;
		return false;
		break;
	}

	// Select the enemy ant at the most perpendicular position to the direction
	switch ((_atkDirection + 1) % 4)
	{
	case 0: /*NORTH*/
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = mostExtremityAnt;
			reEvaluateLocations(loc1, loc2);

			if (loc1._row > loc2._row) {
				mostExtremityAnt = otherEnemyPos;
			}
		}

		break;
	case 1: /*EAST*/
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = mostExtremityAnt;
			reEvaluateLocations(loc1, loc2);

			if (loc1._col < loc2._col) {
				mostExtremityAnt = otherEnemyPos;
			}
		}

		break;
	case 2: /*SOUTH*/
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = mostExtremityAnt;
			reEvaluateLocations(loc1, loc2);

			if (loc1._row < loc2._row) {
				mostExtremityAnt = otherEnemyPos;
			}
		}

		break;
	case 3: /*WEST*/
		for (Location& otherEnemyPos : _otherEnemiesPos)
		{
			Location loc1 = otherEnemyPos;
			Location loc2 = mostExtremityAnt;
			reEvaluateLocations(loc1, loc2);

			if (loc1._col > loc2._col) {
				mostExtremityAnt = otherEnemyPos;
			}
		}
		break;
	default:
		break;
	}

	// If enemies are horizontally positionned, our ants try to form a horizontal line
	if (_directionVec._col == 0)
	{
		//r_gbb._state._bug << "Making an horizontal line : " << closestAntInDirection << std::endl;
		int y = 0;

		for (int i = 0; i < _otherEnemiesPos.size() + 1 - y; i++)
		{
			Location tmpAtkPos = Location((closestAntInDirection._row + (1 * (r_gbb._state._attackRadius + 2) * -_directionVec._row)), mostExtremityAnt._col + i * _directionVec._row);
			if (r_gbb._state._grid[(tmpAtkPos._row + r_gbb._state._rows) % r_gbb._state._rows][(tmpAtkPos._col + r_gbb._state._cols) % r_gbb._state._cols]._isWater)
			{
				y++;
				tmpAtkPos = Location((closestAntInDirection._row + (1 * (r_gbb._state._attackRadius + 2) * -_directionVec._row)), mostExtremityAnt._col - y * _directionVec._row);
				i--;
				if (r_gbb._state._grid[(tmpAtkPos._row + r_gbb._state._rows) % r_gbb._state._rows][(tmpAtkPos._col + r_gbb._state._cols) % r_gbb._state._cols]._isWater) {
					cancel();
					//r_gbb._state._bug << "not enough room for making a line : " << closestAntInDirection << std::endl;
					return false;
				}
			}
			tmpAtkPos._row = (tmpAtkPos._row + r_gbb._state._rows) % r_gbb._state._rows;
			tmpAtkPos._col = (tmpAtkPos._col + r_gbb._state._cols) % r_gbb._state._cols;
			_atkPos.push_back(tmpAtkPos);
		}
	}

	// If enemies are vertically positionned, our ants try to form a vertical line
	if (_directionVec._row == 0)
	{
		//r_gbb._state._bug << "Making an vertical line : " << closestAntInDirection << std::endl;
		int y = 0;
		for (int i = 0; i < _otherEnemiesPos.size() + 1 - y; i++)
		{
			Location tmpAtkPos = Location(mostExtremityAnt._row + i * _directionVec._col, (closestAntInDirection._col + (1 * (r_gbb._state._attackRadius + 2) * -_directionVec._col)));
			if (r_gbb._state._grid[(tmpAtkPos._row + r_gbb._state._rows) % r_gbb._state._rows][(tmpAtkPos._col + r_gbb._state._cols) % r_gbb._state._cols]._isWater)
			{
				y++;
				tmpAtkPos = Location(mostExtremityAnt._row - y * _directionVec._col, (closestAntInDirection._col + (1 * (r_gbb._state._attackRadius + 2) * -_directionVec._col)));
				i--;
				if (r_gbb._state._grid[(tmpAtkPos._row + r_gbb._state._rows) % r_gbb._state._rows][(tmpAtkPos._col + r_gbb._state._cols) % r_gbb._state._cols]._isWater) {
					cancel();
					//r_gbb._state._bug << "not enough room for making a line : " << closestAntInDirection << std::endl;
					return false;
				}
			}
			tmpAtkPos._row = (tmpAtkPos._row + r_gbb._state._rows) % r_gbb._state._rows;
			tmpAtkPos._col = (tmpAtkPos._col + r_gbb._state._cols) % r_gbb._state._cols;
			_atkPos.push_back(tmpAtkPos);
		}
	}

	// TODO : remove debug log
	/*r_gbb._state._bug << "	Atk pos are : " << endl;
	for (Location& atkPos : _atkPos) {
		r_gbb._state._bug << atkPos << endl;
	}*/

	return true;
}

void Reinforcement::cancel()
{
	_isValid = false;
}

int Reinforcement::calculateDirection(const Location& enemy, bool checkingOtherAnts)
{
	Location loc1 = _callingAntPos;
	Location loc2 = enemy;

	reEvaluateLocations(loc1, loc2);

	Location directionVec = Location(loc2._row - loc1._row, loc2._col - loc1._col);

	// If enemy in perfect diagonal, the direction is the average direction of the other enemies
	if (abs(directionVec._col) == abs(directionVec._row) && _otherEnemiesPos.size() > 1 && !checkingOtherAnts)
	{
		int tmpDirection = 0;
		for (Location otherEnemy : _otherEnemiesPos) {
			tmpDirection += calculateDirection(otherEnemy, true);
		}
		int averageDirection = (int)roundf(tmpDirection / (_otherEnemiesPos.size() - 1));
		return averageDirection;
	}

	// Right or left
	if (abs(directionVec._col) > abs(directionVec._row)) {
		if (directionVec._col > 0)
		{
			return 1 /*EAST*/;
		}
		else {
			return 3 /*WEST*/;
		}
	}
	else // Up or Down
	{
		if (directionVec._row > 0) {
			return 2 /*SOUTH*/;
		}
		else {
			return 0 /*NORTH*/;
		}
	}
	return 1;
}

// Use this before comparing 2 Locations to avoid looping bounding issues
void Reinforcement::reEvaluateLocations(Location& loc1, Location& loc2) {
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	int rows = r_gbb._state._rows;
	int cols = r_gbb._state._cols;

	if (loc1._row - loc2._row >= rows / 2)
	{
		loc2._row += rows;
	}
	else if (loc1._row - loc2._row <= -rows / 2) {
		loc1._row += rows;
	}

	if (loc1._col - loc2._col >= cols / 2)
	{
		loc2._col += cols;
	}
	else if (loc1._col - loc2._col <= -cols / 2) {
		loc1._col += cols;
	}
}

bool Reinforcement::isValid()
{
	return _isValid;
}

bool Reinforcement::allAntsInPosition()
{
	if (_antPositionAlreadyChecked)
	{
		return _allAntsInPosition;
	}

	for (Location& atkPos : _atkPos) {
		bool antInAtkPos = false;
		for (Location& helpingAnt : _helpingAntPos) {
			if (atkPos == helpingAnt)
			{
				antInAtkPos = true;
			}
		}
		if (!antInAtkPos) {
			_allAntsInPosition = false;
			_antPositionAlreadyChecked = true;
			return false;
		}
			
	}
	_allAntsInPosition = true;
	_antPositionAlreadyChecked = true;
	return true;
}