#include "InputEnemyNear.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

InputEnemyNear::InputEnemyNear(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputEnemyNear::~InputEnemyNear()
{
}

EStatus InputEnemyNear::checkCondition()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;

	std::vector<Location> nearEnemies = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._enemyAnts, *(currentAnt), 5, 1);
	if (nearEnemies.size() == 0) return BH_FAILURE;
	
	// TODO : Remove debug log entry
	//GlobalBlackboard::singleton()._state._bug << "ENEMY SPOTED : " << "(" << nearEnemies[0]._row << ", " << nearEnemies[0]._col << ")" << std::endl;

	_blackboard->_targetEnemy = nearEnemies[0];
	return BH_SUCCESS;
}
