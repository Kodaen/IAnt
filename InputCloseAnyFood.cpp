#include "InputCloseAnyFood.h"
#include "GlobalBlackboard.h"

InputCloseAnyFood::InputCloseAnyFood(LocalBlackboard& _blackboard) : Input(_blackboard)
{
}

InputCloseAnyFood::~InputCloseAnyFood()
{
}

EStatus InputCloseAnyFood::checkCondition()
{
	EStatus res = BH_FAILURE;

	Location* currentAnt = _blackboard->p_ant;
	auto* nearbyFoodAnts = &GlobalBlackboard::singleton()._nearbyFoodAnts;

	// for each _nearbyFoodAnts pair,check if this ant is nearby any food
	for (std::map<Location, std::vector<Location>>::iterator& food = nearbyFoodAnts->begin(); food != nearbyFoodAnts->end(); ++food) {
		for (Location& ant: food->second)
		{
			if (ant == *currentAnt)
			{
				// put every food of which is ant is nearby in the local blackboard
				_blackboard->_nearbyFood.push_back(food->first);
				// return Success
				res = BH_SUCCESS;
			}
		}

	}

	return res;
}
