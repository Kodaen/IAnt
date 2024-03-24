#include "InputClosestToMyHill.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

InputClosestToMyHill::InputClosestToMyHill(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputClosestToMyHill::~InputClosestToMyHill()
{
}

EStatus InputClosestToMyHill::checkCondition()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Location* currentAnt = _blackboard->p_ant;
	std::pair<Location, int> targetEnemy = std::make_pair(_blackboard->_targetEnemy, r_gbb._state._enemyAntsTeam[_blackboard->_targetEnemy]);

	Location targetEnemyHill = MapSystem::getInstance()->getMostProbableAnthill(targetEnemy.first, targetEnemy.second);
	float distEnemy = MapSystem::getInstance()->findCost(targetEnemy.first, targetEnemyHill);

	Location myHill = MapSystem::getInstance()->getMostProbableAnthill(*currentAnt, 0);
	float distCurrentAnt = MapSystem::getInstance()->findCost(*(currentAnt), myHill);

	if (distCurrentAnt < distEnemy)
	{
		return BH_SUCCESS;
	}
	else {
		return BH_FAILURE;
	}
}
