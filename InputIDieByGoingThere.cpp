#include "InputIDieByGoingThere.h"
#include "GlobalBlackboard.h"

InputIDieByGoingThere::InputIDieByGoingThere(LocalBlackboard& _blackboard) 
	: Input(_blackboard) { }

InputIDieByGoingThere::~InputIDieByGoingThere()
{
}

EStatus InputIDieByGoingThere::checkCondition()
{
	Location& currentAnt = *(_blackboard->p_ant);

	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();

	int dangerRadius = (int)std::floor(r_gbb._state._attackRadius) +1;

	// We search nearby ants in a square of length 16x16
	for (int col = -dangerRadius; col < dangerRadius; col++)
	{
		for (int row = -dangerRadius; row < dangerRadius; row++)
		{
			auto gridRow = (r_gbb._state._rows + (_blackboard->_nextLocation._row + row)) % r_gbb._state._rows;
			auto gridCol = (r_gbb._state._cols + (_blackboard->_nextLocation._col + col)) % r_gbb._state._cols;

			if (r_gbb._state._grid[gridRow][gridCol]._isEnemyAnt) {
				return BH_SUCCESS;
			}
		}
	}

	return BH_FAILURE;
}
