#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"

/*
    constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };      //{N, E, S, W}

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
    */
    int _rows, _cols,
        _turn, _turns,
        _noPlayers;
    double _attackRadius, _spawnRadius, _viewRadius;
    double _loadTime, _turnTime;
    std::vector<double> _scores;
    bool _gameOver;
    int64_t _seed;

    std::vector<std::vector<Square> > _grid;
    std::vector<Location> _myAnts, _enemyAnts, _myHills, _enemyHills, _food;

    Timer _timer;
    Bug _bug;

    /*
        Functions
    */
    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);

    inline double manhattanDistance(const Location &loc1, const Location &loc2);
    inline double euclidianDistance(const Location& loc1, const Location& loc2);
    inline Location getLocation(const Location &startLoc, int direction);
    std::vector<int> getDirections(const Location& loc1, const Location& loc2);
    int getDirection(const Location& loc1, const Location& loc2);

    void updateVisionInformation();
};

std::ostream& operator<<(std::ostream &os, const State &_state);
std::istream& operator>>(std::istream &is, State &_state);

#endif //STATE_H_
