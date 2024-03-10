#include "ActionCalcTrajForFood.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionCalcTrajForFood::ActionCalcTrajForFood(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionCalcTrajForFood::~ActionCalcTrajForFood()
{}

EStatus ActionCalcTrajForFood::doAction()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	_blackboard->_nextLocation = MapSystem::getInstance()->moveToward(*(_blackboard->p_ant), _blackboard->_targetFood);

	return BH_SUCCESS;
}
