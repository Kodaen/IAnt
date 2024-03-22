#include "ActionExplore.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionExplore::ActionExplore(LocalBlackboard& _blackboard) : Action(_blackboard) {}

ActionExplore::~ActionExplore()
{
}

EStatus ActionExplore::doAction()
{
	EStatus res = BH_FAILURE;
	return res;
	auto targetLocation=MapSystem::getInstance()->getOldestVisitedSentinelPoint();
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	GlobalBlackboard::singleton()._state._bug << "Exploring, going to :" << targetLocation << std::endl;
	if (r_gbb.singleton().doMoveLocation(*(_blackboard->p_ant), _blackboard->_nextLocation)) {
		res = BH_SUCCESS;
	}
	
	return res;
}
