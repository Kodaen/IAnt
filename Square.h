#ifndef SQUARE_H_
#define SQUARE_H_

#include <vector>

/*
    struct for representing a square in the grid.
*/
struct Square
{
    bool _isVisible, _isWater, _isHill, _isFood, _isMyAnt, _isEnemyAnt;
    int _ant, _hillPlayer;
    std::vector<int> _deadAnts;

    Square()
    {
        _isVisible = _isWater = _isHill = _isFood = _isMyAnt = _isEnemyAnt = 0;
        _ant = _hillPlayer = -1;
    };

    //resets the information for the square except water information
    void reset()
    {
        _isVisible = 0;
        _isHill = 0;
        _isFood = 0;
        _isMyAnt = 0;
        _isEnemyAnt = 0;
        _ant = _hillPlayer = -1;
        _deadAnts.clear();
    };
};

#endif //SQUARE_H_
