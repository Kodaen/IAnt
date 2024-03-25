#include "MapSystem.h"
#include <bitset>
#include "State.h"

#define MAP_FILE "map.map"
#define MAP_LINE_INDICATOR 'm'
#define CHAR_WALKABLE_CELL '.' //This is the "basic" char for empty cells, special cases are then handled with the "OTHERS_CHAR_EMPTY_CELLS" constant
#define ANT_HILLS_CHARS "0123456789"
#define CHAR_WALL_CELLS '%'
#define COLS_LINE_PREFIX string("cols ")
#define ROWS_LINE_PREFIX string("rows ")

using namespace std;

MapSystem* MapSystem::_instance = nullptr;

MapSystem* MapSystem::getInstance()
{
    if (_instance == nullptr) {
        _instance = new MapSystem();
    }
    return _instance;
}

inline bool Contain(const std::vector<Location>& vec, const Location& loc)
{
    for (auto& l : vec)
    {
		if(l == loc)
			return true;
	}
    return false;
}

void MapSystem::setup()
{
    loadMapFromFile(ifstream(MAP_FILE, ifstream::in));
    auto pathData = findPath(Location(6, 1), Location(43, 3));
    computeSentinelsPoints(8);
   
#if DEBUG
    _bug << endl;
    printMap();
    _bug << endl;

    _bug << endl;
    printSentinelsMap();
    _bug << endl;

    _bug << endl;
    printSentinelsViewMap();
    _bug << endl;
#endif
}

//Returns 0 if cell is not walkable, 1 if it is, 2 if it is an anthill
//This way the output can be treated as a boolean for finding if cell is walkable, but also gives additional informations for ant hills if needed
inline int isWalkableCellChar(char charToCheck) {
    //We proceed step by step to avoid useless costs. If the charToCheck is the "default char for empty cells" or the "default char for walls" we just need to compare 2 chars. 
    // As it will be the most common case, we avoid iterating through a string for each character
    if (charToCheck == CHAR_WALKABLE_CELL)
        return 1;
    if (charToCheck == CHAR_WALL_CELLS)
        return 0;
    if (string(ANT_HILLS_CHARS).find(charToCheck) != string::npos)
        return 2;
}

//Create the graph, fills the various arrays, vectors and data of the class by reading a map file
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
    _colSize=0;
    _rowSize=0;

    //First we need to find the width and height of the map
    while (getline(mapFile, line))
    {
        if (line.substr(0, ROWS_LINE_PREFIX.length()) == ROWS_LINE_PREFIX) 
        {
            _rowSize = stoi(line.substr(ROWS_LINE_PREFIX.length()));
            continue;
        }

        if (line.substr(0, COLS_LINE_PREFIX.length()) == COLS_LINE_PREFIX) 
        {
            _colSize = stoi(line.substr(COLS_LINE_PREFIX.length()));
            break;
		}
    }
    
    _isCellWalkable= std::vector<std::vector<bool>>(_rowSize, std::vector<bool>(_colSize, false));
    std::vector<std::vector<Node<Location>*>> cellNodes(_rowSize, std::vector<Node<Location>*>(_colSize, false));

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
            auto isWalkable = isWalkableCellChar(currentChar);
            _isCellWalkable[row][col] = isWalkableCellChar(currentChar);

            //If "isWalkable" is two, not only was the cell walkable, but it's also an anthill
            if (isWalkable == 2)
            {
                //Substracting "'0'" to a char is the fastest way to convert it to an int
                _unknowAnthills.push_back(Location(row, col));
            }

            col++;
		}
		col = 0;
		row++;
    }

    _bug << "Col size: " << _colSize << " Row size: " << _rowSize << endl;
    //We make a first pass to create a node for each walkable cell
    for (int row = 0; row < _rowSize; row++)
    {
        for (int col = 0; col < _colSize; col++)
        {
            if (!_isCellWalkable[row][col]) continue;
            cellNodes[row][col] = _mapGraph.addNode(Location(row, col));
		}
	}

    //We make a second pass to add neighbours
    for (int row = 0; row < _rowSize; row++)
    {
        for (int col = 0; col < _colSize; col++)
        {
            if (!_isCellWalkable[row][col]) continue;
            int leftCol = (col > 0) ? col - 1 : _colSize - 1;
            int rightCol = (col < _colSize - 1) ? col + 1 : 0;
            int upRow = (row > 0) ? row - 1 : _rowSize - 1;
            int downRow = (row < _rowSize - 1) ? row + 1 : 0;

            if (_isCellWalkable[row][leftCol]) {
                cellNodes[row][col]->addNeighbor(cellNodes[row][leftCol],1);
            }
            if (_isCellWalkable[row][rightCol])
            {
				cellNodes[row][col]->addNeighbor(cellNodes[row][rightCol], 1);
			}
            if (_isCellWalkable[upRow][col])
            {
				cellNodes[row][col]->addNeighbor(cellNodes[upRow][col], 1);
			}
            if (_isCellWalkable[downRow][col])
            {
                cellNodes[row][col]->addNeighbor(cellNodes[downRow][col], 1);
            }
        }
    }

    mapFile.close();
}

Astar::PathData<Location>  MapSystem::findPath(Location from, Location to)
{
    auto heuristic = [this](Location a, Location b) -> float {
        return this->getManhattanDistance(a, b);
    };

    return _mapGraph.findPath(from, to, heuristic);
}

/// <summary>
/// Returns the ant closest to a given point, if there are multiple ants at the same distance, the first one found will be returned in priority
/// </summary>
/// <param name="ants">All the ants elligible to be returned at the end</param>
/// <param name="point">The point we want to find ants closest to</param>
/// <param name="maxDistance">Once we reach this distance, we stop searching</param>
/// <param name="maxAntsNumber">Once we reach this ants number, we stop looking</param>
/// <returns>The vector containing the ants found cloest to the given point, 0 is the closest, the highest index is the furthest</returns>
std::vector<Location> MapSystem::getCloseEnoughAnts(const std::vector<Location>& ants, Location point, int maxDistance, int maxAntsNumber)
{
    auto closeAnts = _mapGraph.findDataOfNodesBetween(point, 0, maxDistance, true, ants, true, maxAntsNumber);
    return closeAnts;
}


/// <summary>
/// Will trace circles bigger and bigger around the origin to find the closest ground and return it
/// </summary>
/// <param name="origin">The wall cell we start from</param>
/// <param name="maxDistance">After this distance (in euclidian terms), we give up</param>
/// <param name="forcedRowDirection">If different from 0, will only look for closest wall in this direction</param>
/// <param name="forcedColDirection">If different from 0, will only look for closest wall in this direction</param>
/// <returns>The closest ground/walkable cell</returns>
/// //The getDistanceBetween function of the graph couldn't be used because here, we're not looking in terms of graph in cost, but in terms of pure geographical distance
Location MapSystem::getClosestGround(Location origin, int maxDistance, int forcedRowDirection, int forcedColDirection)
{
    int minRowMultiplicator = -1;
    int maxRowMultiplicator  = 1;
    int minColMultiplicator  =-1;
    int maxColMultiplicator  = 1;

    if (forcedRowDirection>0)
    {
		minRowMultiplicator = 0;
	}
    else if (forcedRowDirection<0) 
    {
        maxRowMultiplicator = 0;
    }

    if (forcedColDirection>0)
    {
		minColMultiplicator = 0;
	}
    else if (forcedColDirection<0)
    {
		maxColMultiplicator = 0;
	}

    for (int distance = 1; distance <= maxDistance + 1; distance++)
    {
        for (int row = distance*minRowMultiplicator; row <= distance*maxRowMultiplicator; row++)
        {
            for (int col = distance*minColMultiplicator; col <= distance*maxColMultiplicator; col++)
            {
                if (squaredEuclidianDistance(Location(0, 0), Location(row, col))<maxDistance * maxDistance)
                {
                    Location currentPoint = Location((origin._row + row) % _rowSize, (origin._col + col) % _colSize);

                    if (currentPoint._col<0) currentPoint._col += _colSize;
                    if (currentPoint._row<0) currentPoint._row += _rowSize;

                    if (_isCellWalkable[currentPoint._row][currentPoint._col])
                    {
                        return currentPoint;
                    }
                }
            }
        }
    }
    return NULL_LOCATION;
}

/// <summary>
/// The sentinels point are points that "grid" the map, points where explorer should go and from there, see around.
/// The sentinel points should cover the map while avoiding overlap as much as possible
/// </summary>
/// <param name="viewDistance"></param>
void MapSystem::computeSentinelsPoints(const int &viewDistance)
{
    //We'll cut the map into a diagonal grid, allowing sentil points to view the whole map
    //Our goal is to create this kind of pattern, with X boing sentinels points and . being regular cells
    /*
    X.....X.....X
    ...X.....X...
    X.....X.....X
    */
    map<Location, SentinelPoint*> sentinelPointsMap;
    p_tiedSentinelPoints = std::vector<std::vector<SentinelPoint*>>(_rowSize, std::vector<SentinelPoint*>(_colSize, nullptr));
    int maxDistance = viewDistance + 1;
    //We need to have the points evenly spaced on the col axes, but closest as possible to viewDistance*2 so we find the biggest number inferior to view distance to achieve that
    int colNbrOfPoints = ceil(float(_colSize) / float(maxDistance * 2));
    int colDistanceBetweenPoints = _colSize / colNbrOfPoints;
    //We do the same for rows, except here we want to have half the viewDistance as our target
    int rowNbrOfPoints = ceil(float(_rowSize) / float(maxDistance));
    int rowDistanceBetweenPoints = _rowSize / rowNbrOfPoints;

    //Because we have a little less distance than the maximum allowed, it gives us a tolerance that we'll benefit from later
    int colTolerance = maxDistance *2 - colDistanceBetweenPoints;
    int rowTolerance= maxDistance - rowDistanceBetweenPoints;
    
    //Now that everything is calculated, we can add the points
    for (int i = 0; i < colNbrOfPoints; i++)
    {
        for (int j = 0; j < rowNbrOfPoints; j++)
        {
            auto col = i * colDistanceBetweenPoints;
            //One row out of 2 is shifted
            if (j % 2)
            {
                col += colDistanceBetweenPoints / 2;
            }
            Location point = Location(j * rowDistanceBetweenPoints, col);

            //If the sentil point already falls on the ground, no further action required for this point
            if (_isCellWalkable[point._row][point._col])
            {
                auto newSP= new SentinelPoint(point);
                p_sentinelsPoints.push_back(newSP);
                _sentinelPointsLocations.push_back(point);
                sentinelPointsMap[point] = newSP;
                continue;
            }

            //Else, we need to put a sentinel point next to the wall
            auto point1 = getClosestGround(point, viewDistance);
            if (point1 == NULL_LOCATION) continue;
            auto newSP = new SentinelPoint(point1);
            p_sentinelsPoints.push_back(newSP);
            _sentinelPointsLocations.push_back(point1);
            sentinelPointsMap[point1] = newSP;

            int rowDifference = point1._row - point._row;
            int colDifference = point1._col - point._col;
            //Take wrapping into account
            rowDifference = min(rowDifference, _rowSize - rowDifference);
            colDifference = min(colDifference, _colSize - colDifference);

            //If we're close enough to the original point, we don't need to add a new point on the other side of the wall
            if (abs(rowDifference) <= rowTolerance && abs(colDifference) <= colTolerance) continue;
            //_bug << "abs(rowDifference): " << abs(rowDifference) << " rowTolerence: " << rowTolerance << " abs(colDifference): " << abs(colDifference) << "colTolerance " << colTolerance << endl;

            //Else, we'll have to
            auto point2 = getClosestGround(point, viewDistance, -rowDifference, -colDifference);
            if (point2 == NULL_LOCATION) continue;
            newSP = new SentinelPoint(point2);
            p_sentinelsPoints.push_back(newSP);
            _sentinelPointsLocations.push_back(point2);
            sentinelPointsMap[point2] = newSP;
        }
    }
   
    //We need to tie each cell of the map to the closest sentinel point, so we can find the most interesting sentinel point in every situation
    //Based on the last time it was "visited" by an ant. Being visited means that an ant walked on a cell which is tied to a sentinel point
    //We use BFS to find the closest sentinel point for each cell instead of a simple distance calculation because we want to take wrapping into account this time
    for (int row = 0; row < _rowSize; row++)
	{
		for (int col = 0; col < _colSize; col++)
		{
			if (!_isCellWalkable[row][col]) continue;

			auto closestSentinelPoint = _mapGraph.findDataOfNodesBetween(Location(row, col), 0, INT_MAX, true, _sentinelPointsLocations, true, 1);
			if (closestSentinelPoint.empty())
			{
				_bug << "FATAL ERROR: big error in \"computeSentinelsPoints\", the closest sentinel point wasn't found, we've checked the closest sentinel point on the entire map size, and we found nothing, very weird";
				continue;
			}

			p_tiedSentinelPoints[row][col]=sentinelPointsMap[closestSentinelPoint[0]];
		}
	}
}

//Each time our ants see an anthill, we register it in the map system to identify all the anthills as fast as possible
void MapSystem::registerAnthillsSighting(int team, Location antHill)
{
    //If we have no unknow anthills left, we can ignore this sighting
    if (_unknowAnthills.empty()) return;
    _knowAnthills[team] = antHill;
    _unknowAnthills.erase(std::remove(_unknowAnthills.begin(), _unknowAnthills.end(), antHill), _unknowAnthills.end());
    //If we have only one anthill left, we can register it as the anthill of the team
    if (_unknowAnthills.size() == 1)
    {
        _knowAnthills[team] = _unknowAnthills[0];
        _unknowAnthills.clear();
        return;
    }
}

//Return the most probable anthill for a given ant and team
Location MapSystem::getMostProbableAnthill(Location ant, int team)
{
    //If we have found the anthill of the ant's team, we return it
    if (_knowAnthills.find(team) != _knowAnthills.end()) return _knowAnthills[team];
    //If we have not found the anthill of the ant's team, we return the closest anthill
    auto closestAnthills = _mapGraph.findDataOfNodesBetween(ant, 0, INT_MAX, true, _unknowAnthills, true, 1);
    if (closestAnthills.empty())
    {
        _bug << "FATAL ERROR: big error in \"getMostProbableAnthill\", the anthill wasn't known, we've checked the closest ant hill on the entire map size, and we found nothing, very weird";
        return ant;
    }
}

//Must be called for each ant on each turn
void MapSystem::updateSentinelsPoint(Location ant, int turn)
{
     p_tiedSentinelPoints[ant._row][ant._col]->_lastVisit = turn;
}

std::vector<MapSystem::SentinelPoint> MapSystem::getOldestToNewestVisitedSentinelPoint()
{
    std::vector<SentinelPoint> returnVector;
    sort(p_sentinelsPoints.begin(), p_sentinelsPoints.end(), [](SentinelPoint* a, SentinelPoint* b) {return a->_lastVisit < b->_lastVisit; });

    for each (auto sentinelPoint in p_sentinelsPoints)
    {
        returnVector.push_back(*sentinelPoint);
    }

    return returnVector;
}

#if DEBUG
void MapSystem::printMap()
{
    for (int row = 0; row < _rowSize; row++)
    {
        for (int col = 0; col < _colSize; col++)
        {
			if (_isCellWalkable[row][col])
				_bug << CHAR_WALKABLE_CELL;
			else
				_bug << CHAR_WALL_CELLS;
		}
		_bug << endl;
	}
}

void MapSystem::printSentinelsMap()
{
    for (int row = 0; row < _rowSize; row++)
    {
        for (int col = 0; col < _colSize; col++)
        {
            if (std::count(_sentinelPointsLocations.begin(), _sentinelPointsLocations.end(), Location(row, col))) {
                _bug << "%";
                continue;
            }
            _bug << ".";
        }
        _bug << endl;
    }
}

/// <summary>
/// Show what an ant would see, not very accurate, nor optimized
/// </summary>
/// <param name="origin">Origin of the circle, position of the ant</param>
/// <param name="maxDistance">View distance of the ant</param>
/// <returns></returns>
std::vector<Location> MapSystem::getViewCircles(const Location& origin,const int& viewDistance)
{
    std::vector<Location> result;

    for (int distance = 1; distance <= viewDistance + 1; distance++)
    {
        for (int row = -distance; row <= distance; row++)
        {
            for (int col = -distance; col <= distance; col++)
            {
                if (floor(squaredEuclidianDistance(Location(0, 0), Location(row, col))) < ceil(distance * distance))
                {
                    Location currentPoint = Location((origin._row + row) % _rowSize, (origin._col + col) % _colSize);
                    if (currentPoint._col < 0) currentPoint._col += _colSize;
                    if (currentPoint._row < 0) currentPoint._row += _rowSize;
                    if (count(result.begin(), result.end(), currentPoint)) continue;
                    result.push_back(currentPoint);
                }
            }
        }
    }
    return result;
}



/// <summary>
/// Show what the ants would see if they had an ant on each sentinel point
/// 0 means the cells isn't seen by anyone, 1 means an ant see the cell, more than 1 means there are overlaps
/// best case scenario, each cell should be at one, but it's not feasible
/// </summary>
void MapSystem::printSentinelsViewMap()
{
    auto sightMap = std::vector<std::vector<int>>(_rowSize, std::vector<int>(_colSize, 0));
    for each (auto sentinelPoint in p_sentinelsPoints)
    {
        auto fov = getViewCircles(sentinelPoint->_location, 8);
        for each (auto cellInSight in fov)
        {
            sightMap[cellInSight._row][cellInSight._col] += 1;
        }
    }

    for (int row = 0; row < _rowSize; row++)
    {
        for (int col = 0; col < _colSize; col++)
        {
            _bug << sightMap[row][col]<<"-";
        }
        _bug << endl;
    }
}
#endif