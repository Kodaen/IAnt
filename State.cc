#include "State.h"

using namespace std;

//constructor
State::State()
{
    _gameOver = 0;
    _turn = 0;
    _bug.open("./debug.txt");
};

//deconstructor
State::~State()
{
    _bug.close();
};

//sets the state up
void State::setup()
{
    _grid = vector<vector<Square> >(_rows, vector<Square>(_cols, Square()));
};

//resets all non-water squares to land and clears the bots ant vector
void State::reset()
{
    _myAnts.clear();
    _enemyAnts.clear();
    _myHills.clear();
    _enemyHills.clear();
    _food.clear();
    for(int _row=0; _row<_rows; _row++)
        for(int _col=0; _col<_cols; _col++)
            if(!_grid[_row][_col]._isWater)
                _grid[_row][_col].reset();
};

//outputs move information to the engine
void State::makeMove(const Location &loc, int direction)
{
    cout << "o " << loc._row << " " << loc._col << " " << CDIRECTIONS[direction] << endl;

    Location nLoc = getLocation(loc, direction);
    _grid[nLoc._row][nLoc._col]._ant = _grid[loc._row][loc._col]._ant;
    _grid[loc._row][loc._col]._ant = -1;
};

//returns the manhattan distance between two locations with the edges wrapped
double State::distance(const Location &loc1, const Location &loc2)
{
    int d1 = abs(loc1._row-loc2._row),
        d2 = abs(loc1._col-loc2._col),
        dr = min(d1, _rows-d1),
        dc = min(d2, _cols-d2);
    return dr + dc;
};

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &loc, int direction)
{
    return Location( (loc._row + DIRECTIONS[direction][0] + _rows) % _rows,
                     (loc._col + DIRECTIONS[direction][1] + _cols) % _cols );
}

std::vector<int> State::getDirections(const Location& loc1, const Location& loc2)
{
    std::vector<int> directions = std::vector<int>();
    if (loc1._row < loc2._row) {
        if (loc2._row - loc1._row >= _rows / 2)
        {
            directions.push_back(0 /*NORTH*/);
        }
        else
        {
            directions.push_back(2 /*SOUTH*/);
        }
    }
    else if (loc1._row > loc2._row)
    {
        if (loc1._row - loc2._row >= _rows / 2)
        {
            directions.push_back(2 /*SOUTH*/);
        }
        else
        {
            directions.push_back(0 /*NORTH*/);
        }
    }

    if (loc1._col < loc2._col) {
        if (loc2._col - loc1._col >= _cols / 2)
        {
            directions.push_back(3 /*WEST*/);
        }
        else
        {
            directions.push_back(1 /*EAST*/);
        }
    }
    else if (loc1._col > loc2._col)
    {
        if (loc1._col - loc2._col >= _cols / 2)
        {
            directions.push_back(1 /*EAST*/);
        }
        else
        {
            directions.push_back(3 /*WEST*/);
        }
    }
    return directions;
}
;

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void State::updateVisionInformation()
{
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for(int a=0; a<(int) _myAnts.size(); a++)
    {
        sLoc = _myAnts[a];
        locQueue.push(sLoc);

        std::vector<std::vector<bool> > visited(_rows, std::vector<bool>(_cols, 0));
        _grid[sLoc._row][sLoc._col]._isVisible = 1;
        visited[sLoc._row][sLoc._col] = 1;

        while(!locQueue.empty())
        {
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc, d);

                if(!visited[nLoc._row][nLoc._col] && distance(sLoc, nLoc) <= _viewRadius)
                {
                    _grid[nLoc._row][nLoc._col]._isVisible = 1;
                    locQueue.push(nLoc);
                }
                visited[nLoc._row][nLoc._col] = 1;
            }
        }
    }
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream &os, const State &_state)
{
    for(int _row=0; _row<_state._rows; _row++)
    {
        for(int _col=0; _col<_state._cols; _col++)
        {
            if(_state._grid[_row][_col]._isWater)
                os << '%';
            else if(_state._grid[_row][_col]._isFood)
                os << '*';
            else if(_state._grid[_row][_col]._isHill)
                os << (char)('A' + _state._grid[_row][_col]._hillPlayer);
            else if(_state._grid[_row][_col]._ant >= 0)
                os << (char)('a' + _state._grid[_row][_col]._ant);
            else if(_state._grid[_row][_col]._isVisible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
};

//input function
istream& operator>>(istream &is, State &_state)
{
    int _row, _col, player;
    string inputType, junk;

    //finds out which turn it is
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            _state._gameOver = 1;
            break;
        }
        else if(inputType == "turn")
        {
            is >> _state._turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }

    if(_state._turn == 0)
    {
        //reads game parameters
        while(is >> inputType)
        {
            if(inputType == "loadtime")
                is >> _state._loadTime;
            else if(inputType == "turntime")
                is >> _state._turnTime;
            else if(inputType == "rows")
                is >> _state._rows;
            else if(inputType == "cols")
                is >> _state._cols;
            else if(inputType == "turns")
                is >> _state._turns;
            else if(inputType == "player_seed")
                is >> _state._seed;
            else if(inputType == "viewradius2")
            {
                is >> _state._viewRadius;
                _state._viewRadius = sqrt(_state._viewRadius);
            }
            else if(inputType == "attackradius2")
            {
                is >> _state._attackRadius;
                _state._attackRadius = sqrt(_state._attackRadius);
            }
            else if(inputType == "spawnradius2")
            {
                is >> _state._spawnRadius;
                _state._spawnRadius = sqrt(_state._spawnRadius);
            }
            else if(inputType == "ready") //end of parameter input
            {
                _state._timer.start();
                break;
            }
            else    //unknown line
                getline(is, junk);
        }
    }
    else
    {
        //reads information about the current turn
        while(is >> inputType)
        {
            if(inputType == "w") //water square
            {
                is >> _row >> _col;
                _state._grid[_row][_col]._isWater = 1;
            }
            else if(inputType == "f") //food square
            {
                is >> _row >> _col;
                _state._grid[_row][_col]._isFood = 1;
                _state._food.push_back(Location(_row, _col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> _row >> _col >> player;
                _state._grid[_row][_col]._ant = player;
                if (player == 0) {
                    _state._grid[_row][_col]._isMyAnt = 1;
                    _state._myAnts.push_back(Location(_row, _col));
                }
                else {
                    _state._grid[_row][_col]._isEnemyAnt = 1;
                    _state._enemyAnts.push_back(Location(_row, _col));
                }
                    
            }
            else if(inputType == "d") //dead ant square
            {
                is >> _row >> _col >> player;
                _state._grid[_row][_col]._deadAnts.push_back(player);
            }
            else if(inputType == "h")
            {
                is >> _row >> _col >> player;
                _state._grid[_row][_col]._isHill = 1;
                _state._grid[_row][_col]._hillPlayer = player;
                if(player == 0)
                    _state._myHills.push_back(Location(_row, _col));
                else
                    _state._enemyHills.push_back(Location(_row, _col));

            }
            else if(inputType == "players") //player information
                is >> _state._noPlayers;
            else if(inputType == "scores") //score information
            {
                _state._scores = vector<double>(_state._noPlayers, 0.0);
                for(int p=0; p<_state._noPlayers; p++)
                    is >> _state._scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(_state._gameOver)
                    is.setstate(std::ios::failbit);
                else
                    _state._timer.start();
                break;
            }
            else //unknown line
                getline(is, junk);
        }
    }

    return is;
};
