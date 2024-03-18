#include "Reinforcement.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

Reinforcement::Reinforcement(Location callingAntPos, Location enemyPos)
	: _callingAntPos(callingAntPos), _enemyPos(enemyPos)
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
