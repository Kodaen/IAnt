#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
	//reads the game parameters and sets up
	cin >> state;
	state.setup();
	endTurn();

	//continues making moves while the game is not over
	while (cin >> state)
	{
		state.updateVisionInformation();
		makeMoves();
		endTurn();
	}
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
	state.bug << "turn " << state.turn << ":" << endl;
	state.bug << state << endl;

	orders->clear();

	//picks out moves for each ant
	for (int ant = 0; ant < (int)state.myAnts.size(); ant++)
	{
		for (int d = 0; d < TDIRECTIONS; d++)
		{
			if (doMoveDirection(state.myAnts[ant], d)) {
				break;
			}
		}
	}

	state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

boolean Bot::doMoveDirection(const Location& antLoc, int direction) {
	Location newLoc = state.getLocation(antLoc, direction);

	if (!state.grid[newLoc.row][newLoc.col].isWater && !doesAnotherAndWantToGoThere(newLoc))
	{
		state.makeMove(antLoc, direction);
		orders->insert({ newLoc, antLoc });
		return true;
	}
	else {
		return false;
	}
}


bool Bot::doesAnotherAndWantToGoThere(Location tile)
{
	for (std::map<Location, Location>::iterator it = orders->begin(); it != orders->end(); ++it) {
		if (it->first.col == tile.col && it->first.row == tile.row)
		{
			state.bug << "Tile is already visited !" << endl;
			return true;
		}
	}
	return false;
}

void Bot::printOrders()
{
	state.bug << "Tiles the ants want to go to : " << endl;
	for (std::map<Location, Location>::iterator it = orders->begin(); it != orders->end(); ++it)
		state.bug << it->first.col << " " << it->first.row << endl;
}

//finishes the turn
void Bot::endTurn()
{
	if (state.turn > 0)
		state.reset();
	state.turn++;

	cout << "go" << endl;
};
