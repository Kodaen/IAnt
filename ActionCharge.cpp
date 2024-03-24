#include "ActionCharge.h"
#include "GlobalBlackboard.h"

ActionCharge::ActionCharge(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionCharge::~ActionCharge()
{
}

EStatus ActionCharge::doAction()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;

	Location enemyDirection = _blackboard->_reinforcement->_directionVec;
	Location nextLocation = Location(currentAnt->_row + enemyDirection._row , currentAnt->_col + enemyDirection._col);

	r_gbb.doMoveLocation(*(currentAnt), nextLocation);
	r_gbb._state._bug << "Ant : " << *(currentAnt) << "Charging Enemies" << nextLocation <<std::endl;
	return BH_SUCCESS;
}
