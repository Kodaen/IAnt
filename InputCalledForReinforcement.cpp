#include "InputCalledForReinforcement.h"
#include "GlobalBlackboard.h"

InputCalledForReinforcement::InputCalledForReinforcement(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputCalledForReinforcement::~InputCalledForReinforcement()
{
}

EStatus InputCalledForReinforcement::checkCondition()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;

	for (Reinforcement& reinforcement : r_gbb._reinforcements) {
		for (Location& helpingAnt : reinforcement._helpingAntPos) {
			if (*(currentAnt) == helpingAnt) {
				_blackboard->_reinforcement = &reinforcement;
				return BH_SUCCESS;
			}
		}
	}

	return BH_FAILURE;
}
