#include "InputClosestToFood.h"
#include "GlobalBlackboard.h"

InputClosestToFood::InputClosestToFood(LocalBlackboard& _blackboard) : Input(_blackboard)
{
}

InputClosestToFood::~InputClosestToFood()
{
}

EStatus InputClosestToFood::checkCondition()
{
	EStatus res = BH_FAILURE;

	float tmpClosestDistance = 999;
	for (NearbyFoodAnts& NFA : GlobalBlackboard::singleton()._nearbyFoodAnts) {
		if (*(_blackboard->p_ant) == NFA.closestAntLoc) {
			res = BH_SUCCESS;
			
			if (NFA.closestDistance < tmpClosestDistance) {
				tmpClosestDistance = NFA.closestDistance;
				_blackboard->_targetFood = NFA.foodLoc;
			}

		}
	}

	return res;
}
