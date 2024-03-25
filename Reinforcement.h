#pragma once

#include "Location.h"
#include <vector>

class Reinforcement
{
	// ------------ CONSTRUCTOR ------------ //
public:
	Reinforcement(Location callingAntPos,Location enemyPos, int helpRadius);

	Reinforcement(Location callingAntPos, Location enemyPos);

	Reinforcement();

	// ------------ ATTRIBUTES ------------ //
public:
	// The enemy we are targeting
	Location _enemyPos;
	// Enemies in attack range of enemy we are targeting (_enemyPos)
	std::vector<Location> _otherEnemiesPos;
	// Ants who want to come help for the fight
	std::vector<Location> _helpingAntPos;

	// Direction from which we are attacking
	Location _directionVec;

	// Positions for attacking the enemies
	std::vector<Location> _atkPos;

private:
	// Ant who called for help
	Location _callingAntPos;
	


	// Direction from which we are attacking
	int _atkDirection;

	// Determine how far we look for help
	int _helpRadius;

	// When reinforcement is cancelled, it is no longer valid
	bool _isValid;

	bool _allAntsInPosition;
	bool _antPositionAlreadyChecked;

	// ------------ FUNCTIONS ------------ //
public :
	/// <summary>
	/// Ask near ants if they can help for the fight. And if they can, we put them in helpingAntPos.
	/// </summary>
	/// <returns>
	/// If we were able to find enough allies for help, returns true, else returns false
	/// </returns>
	bool tryAskingHelp();

	/// <summary>
	/// Try setting up the location where each ant should be before the start of the fight.
	/// </summary>
	/// <returns>
	/// If we found a possible atk pos for each ant who participate in the fight, returns true, else return false
	/// </returns>
	bool trySetupAtkPos();

	/// <summary>
	/// If not enough allies could help (because not enough allies nearby or not enough room), then we cancel
	/// the reinforcement.
	/// </summary>
	void cancel();

	int calculateDirection(const Location& enemy, bool checkingOtherAnts);
	void reEvaluateLocations(Location& loc1, Location& loc2);

	bool isValid();
	bool allAntsInPosition();
};