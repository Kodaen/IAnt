#pragma once
#include <set>
#include <functional>
#include <map>
#include <queue>

#include "Node.h"
#include "Bug.h"

#define NULL_LOCATION Location(-1, -1)
namespace Astar {
	template<class T>
	struct PathData
	{
		float _cost;
		bool _isValid;

		PathData(float cost):_isValid(true), _cost(cost) {}
		//This constructor will probably only be used to make invalid paths, so it could just set isValid to false no questions asked, but I thought that no parameter at all could be confusing
		PathData(bool isValid):_isValid(isValid), _cost(0) {}
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

		PathData<T> findPath(T from, T to, std::function<float(T,T)> heuristic)
		{
			if(!_nodes.count(from)) return PathData<T>(false);
			if(!_nodes.count(to)) return PathData<T>(false);
			Node<T>* departureNode = _nodes[from];
			Node<T>* destinationNode =_nodes[to];
			std::priority_queue<Node<T>*> frontier;
			frontier.push(departureNode);
			std::map<T, T> cameFrom;
			cameFrom[from] = NULL_LOCATION;
			std::map<T, float> costSoFar;
			costSoFar[from] = 0;

			_bug << "frontier empty ? " << frontier.empty() << std::endl;
			while (!frontier.empty())
			{
				Node<T>* current = frontier.top();
				frontier.pop();

				_bug << "Current: " << current->getData() << std::endl;
				_bug << "neighbors number ? " << current->getNeighbors().size() << std::endl;
				for each (Neighbor<T> neighbor in current->getNeighbors()) 
				{
					_bug << "--\n";
					_bug << "Current: " << current->getData() << "   Neighbor:" << neighbor._node->getData() << std::endl;
					_bug << "	neighbor._node == current ?" << (neighbor._node == current) << std::endl;
					if (neighbor._node == current) continue;
					float newCost = costSoFar[current->getData()] + neighbor._costToReach;
					//If the path to the evaluated node is either non existent or more costly, we register the new path
					_bug << "	costSoFar.count(neighbor._node->getData()) " << costSoFar.count(neighbor._node->getData()) << std::endl;
					_bug << "	costSoFar[neighbor._node->getData()] " << costSoFar[neighbor._node->getData()] << std::endl;
					_bug << "	newCost " << newCost << std::endl;
					if (costSoFar.count(neighbor._node->getData()) && costSoFar[neighbor._node->getData()] < newCost) continue;
					
					neighbor._node->_priority = newCost + heuristic(destinationNode->getData(), neighbor._node->getData());
					costSoFar[neighbor._node->getData()] = newCost;
					_bug << "	newCost: " << newCost << std::endl;
					cameFrom[neighbor._node->getData()] = current->getData();
					frontier.push(neighbor._node);
					_bug << "frontier empty (just after pushing) ? " << frontier.empty() << std::endl;
				}
				_bug << "--\n";
				_bug << "frontier empty ? " << frontier.empty() << std::endl;
			}

			_bug<<"cost so far: "<<costSoFar[to] << std::endl;
			return PathData<T>(costSoFar[to]);
		}
	};
}

