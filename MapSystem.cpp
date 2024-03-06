#include "MapSystem.h"
#include <bitset>

#define MAP_FILE "map.map"
#define MAP_LINE_INDICATOR 'm'
#define CHAR_WALKABLE_CELL '.' //This is the "basic" char for empty cells, special cases are then handled with the "OTHERS_CHAR_EMPTY_CELLS" constant
#define OTHERS_CHAR_WALKABLE_CELLS "0123456789"
#define CHAR_WALL_CELLS '%'
#define COLS_LINE_PREFIX string("cols ")
#define ROWS_LINE_PREFIX string("rows ")

using namespace std;

MapSystem::MapSystem()
{
    _bug.open("./mapSystemDebug.txt");
}

MapSystem::~MapSystem()
{
    _bug.close();
}

void MapSystem::setup()
{
    loadMapFromFile(ifstream(MAP_FILE, ifstream::in));
}

//Returns true if the char is a empty cell char
inline bool isEmptyCellChar(char charToCheck) {
    //We proceed step by step to avoid useless costs. If the charToCheck is the "default char for empty cells" or the "default char for walls" we just need to compare 2 chars. 
    // As it will be the most common case, we avoid iterating through a string for each character
    if (charToCheck == CHAR_WALKABLE_CELL)
        return true;
    if (charToCheck == CHAR_WALL_CELLS)
        return false;
    if (string(OTHERS_CHAR_WALKABLE_CELLS).find(charToCheck) != string::npos)
        return true;
}

void MapSystem::loadMapFromFile(ifstream mapFile) 
{
    if (mapFile.fail()) 
    {
        _bug << "Map file not found, read the \"HOW TO BUILD\" of the README to find more informations" << endl ;
        throw "Map file not found, read the \"HOW TO BUILD\" of the README to find more informations";
    }

    string line;
    int col = 0;
    int row = 0;
    _width=0;
    _height=0;

    //First we need to find the width and height of the map
    while (getline(mapFile, line))
    {
        if (line.substr(0, ROWS_LINE_PREFIX.length()) == ROWS_LINE_PREFIX) 
        {
            _height = stoi(line.substr(ROWS_LINE_PREFIX.length()));
            continue;
        }

        if (line.substr(0, COLS_LINE_PREFIX.length()) == COLS_LINE_PREFIX) 
        {
            _width = stoi(line.substr(COLS_LINE_PREFIX.length()));
            break;
		}
    }
    
    //Equivalent of a 2D array filled with false,  each cell's status will be accessible by typing isCellWalkable[col][row]
    std::vector<std::vector<bool>> isCellWalkable(_width, std::vector<bool>(_height, false));
    std::vector<std::vector<Node<Location>*>> cellNodes(_width, std::vector<Node<Location>*>(_height, false));

    //Then we need to fill the 2D array with the map's content
    while (getline(mapFile, line))
    {
        if(line[0]!=MAP_LINE_INDICATOR)
			continue;

        for each(char currentChar in line)
        {
            if (currentChar == MAP_LINE_INDICATOR) continue;
            if (currentChar == ' ') continue;
            //_bug << "col "<<col<<" row "<<row << " \"" << currentChar<<"\""<< endl;
            isCellWalkable[col][row] = isEmptyCellChar(currentChar);
            col++;
		}
		col = 0;
		row++;
    }

    _bug << "Map width: " << _width << " Map height: " << _height << endl;
    //We make a first pass to create a node for each walkable cell
    for (int row = 0; row < _height; row++)
    {
        for (int col = 0; col < _width; col++)
        {
            if (!isCellWalkable[col][row]) continue;
            cellNodes[col][row] = _mapGraph.addNode(Location(col, row));
		}
	}

    //We make a second pass to add neighbours
    for (int row = 0; row < _height; row++)
    {
        for (int col = 0; col < _width; col++)
        {
            if (!isCellWalkable[col][row]) continue;
            bool leftCol = (col > 0) ? col - 1 : _width - 1;
            bool rightCol = (col < _width - 1) ? col + 1 : 0;
            bool upRow = (row > 0) ? row - 1 : _height - 1;
            bool downRow = (row < _height - 1) ? row + 1 : 0;

            if (isCellWalkable[leftCol][row]) {
                cellNodes[col][row]->addNeighbor(cellNodes[leftCol][row],1);
            }
            if (isCellWalkable[rightCol][row])
            {
				cellNodes[col][row]->addNeighbor(cellNodes[rightCol][row], 1);
			}
            if (isCellWalkable[col][upRow])
            {
				cellNodes[col][row]->addNeighbor(cellNodes[col][upRow], 1);
			}
            if (isCellWalkable[col][downRow])
            {
                cellNodes[col][row]->addNeighbor(cellNodes[col][downRow], 1);
            }
        }
    }


    mapFile.close();
}


Astar::PathData<Location>  MapSystem::findPath(Location from, Location to)
{
    return _mapGraph.findPath(from, to, &getManhattanDistance);
}


