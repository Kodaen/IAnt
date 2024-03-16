#include "ActionGetClosestFood.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionGetClosestFood::ActionGetClosestFood(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}


ActionGetClosestFood::~ActionGetClosestFood()
{
}

EStatus ActionGetClosestFood::doAction()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;

	std::vector<Location> closestFood = MapSystem::getInstance()->getCloseEnoughAnts(r_gbb._state._food, *(currentAnt), 15, 3);
	if (closestFood.size() == 0) return BH_FAILURE;

	GlobalBlackboard::singleton()._state._bug << "I'm closest to : " << "Food(" << closestFood[0]._row << ", " << closestFood[0]._col << ")" << std::endl;

	_blackboard->_targetFood = closestFood[0];
	return BH_SUCCESS;
}
