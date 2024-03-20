#include "InputWeBothDie.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

InputWeBothDie::InputWeBothDie(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputWeBothDie::~InputWeBothDie()
{
}

EStatus InputWeBothDie::checkCondition()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;
	Location targetEnemy = _blackboard->_targetEnemy;
	float attackRadius = r_gbb._state._attackRadius;

	std::vector<Location> nearAlly = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._myAnts, *(currentAnt), attackRadius - 1, 1);
	std::vector<Location> nearEnemies = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._enemyAnts, targetEnemy, attackRadius, 1);

	// If we are both solo, then success
	if (nearEnemies.size() == 0 && nearAlly.size() == 0) {
		// TODO : Remove debug log entry
		//GlobalBlackboard::singleton()._state._bug << "ENEMY SOLO (no allies or no enemy near): " << "(" << targetEnemy._row << ", " << targetEnemy._col << ")" << std::endl;
		return BH_SUCCESS;
	}

	// TODO : Remove debug log entry
	//GlobalBlackboard::singleton()._state._bug << "ENEMY NONOT SOLO (allies or enemies near): " << "(" << targetEnemy._row << ", " << targetEnemy._col << ")" << std::endl;
	
	// we are more than 1 ant or they are more than 1 ant
	return BH_FAILURE;
}
