#include "InputAllAntsInPosition.h"
#include "GlobalBlackboard.h"

InputAllAntsInPosition::InputAllAntsInPosition(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputAllAntsInPosition::~InputAllAntsInPosition()
{
}

EStatus InputAllAntsInPosition::checkCondition()
{
	if (_blackboard->_reinforcement->allAntsInPosition())
		return BH_SUCCESS;
	else
		return BH_FAILURE;
}
