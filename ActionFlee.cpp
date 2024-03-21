#include "ActionFlee.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"


ActionFlee::ActionFlee(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionFlee::~ActionFlee()
{
}

EStatus ActionFlee::doAction()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;
	// TODO : remove debug log
	//GlobalBlackboard::singleton()._state._bug << "FLEEING from enemy : " << _blackboard->_reinforcement._enemyPos << std::endl;

	if (r_gbb.singleton().doMoveLocation(*(_blackboard->p_ant), r_gbb._state._myHills[0])) {
		return BH_SUCCESS;
	}
	else {
		return BH_FAILURE;
	}
}
