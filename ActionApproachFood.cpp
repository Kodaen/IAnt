#include "ActionApproachFood.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionApproachFood::ActionApproachFood(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionApproachFood::~ActionApproachFood()
{}

EStatus ActionApproachFood::doAction()
{
	EStatus res = BH_FAILURE;

	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location nextLocation = MapSystem::getInstance()->moveToward(*(_blackboard->p_ant), _blackboard->_targetFood);
	//int nextDirection = r_gbb.singleton()._state.getDirection(*(_blackboard->p_ant), nextLocation);

	// TODO : r_gbb.singleton().doMoveLocation(*(_blackboard->p_ant), nextLocation);
	if (r_gbb.singleton().doMoveLocation(*(_blackboard->p_ant), nextLocation)) {
		res = BH_SUCCESS;
	}

	// Tell other ants we are eating it
	for (int i = 0; i < r_gbb._nearbyFoodAnts.size(); i++)
	{
		if (r_gbb._nearbyFoodAnts[i].foodLoc == _blackboard->_targetFood)
		{
			r_gbb._nearbyFoodAnts.erase(r_gbb._nearbyFoodAnts.begin() + i);
		}
	}

	return res;
}
