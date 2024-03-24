#include "ActionPositionMyself.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionPositionMyself::ActionPositionMyself(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionPositionMyself::~ActionPositionMyself()
{
}

EStatus ActionPositionMyself::doAction()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;

	// Check if ant already on atkPos
	for (int i = _blackboard->_reinforcement->_atkPos.size() - 1; i >= 0; i--)
	{
		if (*currentAnt == _blackboard->_reinforcement->_atkPos[i])
		{
			Location test = Location(currentAnt->_row, currentAnt->_col);
			r_gbb.standStill(test);

			_blackboard->_reinforcement->_atkPos.erase(_blackboard->_reinforcement->_atkPos.begin() + i);

			// TODO : remove debug log
			// r_gbb._state._bug << "Ant : " << *currentAnt << " is not moving : " << *currentAnt << endl;
			return BH_SUCCESS;
		}
	}


	// Look for the closest atkPos and go toward it
	std::vector<Location> closestAtkPos = MapSystem::getInstance()->getCloseEnoughAnts(_blackboard->_reinforcement->_atkPos, *(currentAnt), 7, 1);
	
	if (closestAtkPos.size() == 0) {
		r_gbb._state._bug << "couldn't find atk pos" << endl;
		return BH_FAILURE;
	}

	Location nextLoc = MapSystem::getInstance()->moveToward(*currentAnt, closestAtkPos[0]);

	if (r_gbb.singleton().doMoveLocation(*(_blackboard->p_ant), nextLoc)) {
		// If we can go there, we remove the atkpos so the other ants don't try to go there
		for (int i = _blackboard->_reinforcement->_atkPos.size() - 1; i >= 0; i--)
		{
			if (closestAtkPos[0] == _blackboard->_reinforcement->_atkPos[i])
			{
				// TODO : remove debug log
				// r_gbb._state._bug << "Ant : " << *currentAnt << " is moving : " << closestAtkPos[0] << endl;
				_blackboard->_reinforcement->_atkPos.erase(_blackboard->_reinforcement->_atkPos.begin() + i);
				break;
			}
		}
		return BH_SUCCESS;
	}
	else {
		return BH_FAILURE;
	}
}
