#include "Reinforcement.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

Reinforcement::Reinforcement(Location callingAntPos, Location enemyPos, int helpRadius)
	: _callingAntPos(callingAntPos), _enemyPos(enemyPos), _helpRadius(helpRadius)
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
	: Reinforcement(callingAntPos, enemyPos,8) { }

bool Reinforcement::tryAskingHelp()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	// Get all ally ants nearby
	std::vector<Location> closeAllyAnts = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._myAnts, _callingAntPos, _helpRadius, _otherEnemiesPos.size()+10);

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
