#pragma once

#include "Action.h"
#include "GlobalBlackboard.h"

class ActionBlackboardInfo : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionBlackboardInfo(LocalBlackboard& _blackboard) : Action(_blackboard) {}
	~ActionBlackboardInfo() {}

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction() { 
		GlobalBlackboard& r_globalBlackboard = GlobalBlackboard::singleton();
		
		r_globalBlackboard._state._bug << "Current ant : (" << (_blackboard->p_ant)->_row << "," << (_blackboard->p_ant)->_col << ")" << std::endl;

		//r_globalBlackboard._state._bug << "Nearby foods :" << std::endl;
		//for (NearbyFoodAnts& NFA : _blackboard->_nearbyFood) {
		//	r_globalBlackboard._state._bug << "(" << NFA.foodLoc._row << "," << NFA.foodLoc._col << ")" << std::endl;
		//}

		r_globalBlackboard._state._bug << "Target foods : (" << _blackboard->_targetFood._row << "," << _blackboard->_targetFood._col << ")" << std::endl;


		return BH_SUCCESS;
	};
};