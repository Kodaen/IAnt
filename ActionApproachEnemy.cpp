#include "ActionApproachEnemy.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionApproachEnemy::ActionApproachEnemy(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionApproachEnemy::~ActionApproachEnemy()
{
}

EStatus ActionApproachEnemy::doAction()
{
	EStatus res = BH_FAILURE;

	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();

	_blackboard->_nextLocation = MapSystem::getInstance()->moveToward(*(_blackboard->p_ant), _blackboard->_targetEnemy);

	if (r_gbb.singleton().doMoveLocation(*(_blackboard->p_ant), _blackboard->_nextLocation)) {
		GlobalBlackboard::singleton()._state._bug << "APPROACHING ENEMY : " << _blackboard->_targetEnemy << std::endl;
		res = BH_SUCCESS;
	}
	else {
		// TODO : remove debug log
		GlobalBlackboard::singleton()._state._bug << "NOT APPROACHING ENEMY : " << _blackboard->_targetEnemy << std::endl;
	}


	return res;
}
