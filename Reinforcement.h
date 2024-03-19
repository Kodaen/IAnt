#pragma once

#include "Location.h"
#include <vector>

class Reinforcement
{
	// ------------ CONSTRUCTOR ------------ //
public:
	Reinforcement(Location callingAntPos,Location enemyPos, int helpRadius);

	Reinforcement(Location callingAntPos, Location enemyPos);

	// ------------ ATTRIBUTES ------------ //
public:
	// The enemy we are targeting
	Location _enemyPos;
	// Enemies in attack range of enemy we are targeting (_enemyPos)
	std::vector<Location> _otherEnemiesPos;

private:
	// Ant who called for help
	Location _callingAntPos;
	// Ants who want to come help for the fight
	std::vector<Location> _helpingAntPos;
	
	// Positions for attacking the enemies
	std::vector<Location> _atkPos;

	// Direction from which we are attacking
	int _atkDirection;

	// Determine how far we look for help
	int _helpRadius;

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
};