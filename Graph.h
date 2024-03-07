#pragma once
#include <set>
#include <functional>
#include <map>
#include <queue>
#include <vector>

#include "Node.h"
#include "Bug.h"

#define NULL_LOCATION Location(-1, -1)
#define INVALID_PATH_COST 99999999

namespace Astar {
	/*PathData is the structure used to communicate the path to the outside software layer. It contains the cost of the path, and the path itself.
	The path is stored in reverse order, allowing the outside layer to use "pop_back" to get the next node in the path. And this layer to use "push_back" when building the path, which
	is slightly less costly than using insert.*/
	template<class T>
	struct PathData
	{
		float _cost;
		bool _isValid;
		//The path in the reverse order: starts with the destination, end with the departure.
		std::vector<T> _reversePath;

		PathData(std::vector<T> reversePath, float cost) :_reversePath(reversePath), _isValid(true), _cost(cost) {}
		//This constructor will probably only be used to make invalid paths, so it could just set isValid to false no questions asked, but I thought that no parameter at all could be confusing
		PathData(bool isValid) :_isValid(isValid), _cost(0) 
		{
			if (!isValid)
			{
				_cost = INVALID_PATH_COST;
			}
		}
	};


	template<class T>
	class Graph
	{
		std::map<T, Node<T>*> _nodes;
	public:
		Bug _bug;
		Graph()
		{
			_bug.open("graphSystemDebug.txt");
		};
		~Graph()
		{
			for each (auto node in _nodes)
			{
				delete node.second;
			}
			_bug.close();
		};

		Node<T>* addNode(const T value, std::vector<Neighbor<T>> neighbors)
		{
			return _nodes[value] = new Node<T>(value, neighbors);
		}

		Node<T>* addNode(const T node)
		{
			return addNode(node, std::vector<Neighbor<T>>());
		}

		PathData<T> findPath(T from, T to, std::function<float(T, T)> heuristic)
		{
			if (!_nodes.count(from)) return PathData<T>(false);
			if (!_nodes.count(to)) return PathData<T>(false);
			Node<T>* departureNode = _nodes[from];
			Node<T>* destinationNode = _nodes[to];
			std::priority_queue<Node<T>*> frontier;
			frontier.push(departureNode);
			std::map<T, T> cameFrom;
			cameFrom[from] = NULL_LOCATION;
			std::map<T, float> costSoFar;
			costSoFar[from] = 0;
			bool pathFound = false;

			_bug << "frontier empty ? " << frontier.empty() << std::endl;
			while (!frontier.empty())
			{

				Node<T>* current = frontier.top();
				frontier.pop();

				_bug << "Current: " << current->getData() << std::endl;
				for each (Neighbor<T> neighbor in current->getNeighbors())
				{
					if (neighbor._node == current) continue;
					float newCost = costSoFar[current->getData()] + neighbor._costToReach;
					//If the path to the evaluated node is either non existent or more costly, we register the new path
					if (costSoFar.count(neighbor._node->getData()) && costSoFar[neighbor._node->getData()] <= newCost) continue;

					costSoFar[neighbor._node->getData()] = newCost;
					cameFrom[neighbor._node->getData()] = current->getData();
					if (neighbor._node == destinationNode) 
					{
						pathFound = true;
						break;
					}

					neighbor._node->_priority = -(newCost + heuristic(destinationNode->getData(), neighbor._node->getData()));
					frontier.push(neighbor._node);
				}
				if (pathFound) break;
			}

			vector<T> reversePath;
			reversePath.push_back(to);
			auto previousLocation = cameFrom[to];
			while(previousLocation!=from){	
				reversePath.push_back(previousLocation);
				previousLocation = cameFrom[previousLocation];
			}

			for each (auto var in reversePath)
			{
				_bug << "Path: " << var << std::endl;
			}
			
			return PathData<T>(reversePath, costSoFar[to]);
		}
	};
}

