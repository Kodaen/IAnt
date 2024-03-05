#pragma once
#include <set>
#include <functional>
#include <map>

#include "Node.h"
namespace Astar {
	template<class T>
	class Graph
	{
		std::map<T, Node<T>*> _nodes;
	public: 
		Graph() {};
		~Graph() 
		{
			for each (auto node in _nodes)
			{
				delete node.second;
			}
		};

		void addNode(const T node, std::vector<Neighbor<T>> neighbors)
		{
			_nodes[node]=new Node<T>(node,neighbors);
		}

		void addNode(const T node)
		{
			addNode(node, std::vector<Neighbor<T>>());
		}

		T getPath(T from, T to, std::function<float(T,T)> heuristic) 
		{
			Node<T> departureNode = _nodes[from];
			Node<T> destinationNode =_nodes[to];
			std::priority_queue<T> frontier;
			frontier.push(departureNode);
			std::map<T, T> cameFrom;
			cameFrom[from] = null;
			std::map<T, float> costSoFar;
			costSoFar[from] = 0;

			while (!frontier.empty())
			{
				T current = frontier.top();
				frontier.pop();

				for each (Neighbor<T> neighbor in _nodes[current].getNeighbors()) 
				{
					if (neighbor._node == current) continue;
					float newCost = costSoFar[current] + neighbor._costToReach;
					//If the path to the evaluated node is either non existent or more costly, we register the new path
					if (costSoFar.contains(current) != costSoFar.end() && costSoFar[neighbor._node] < newCost) continue;
					
					float priority = costSoFar + heuristic(destinationNode.getData, neighbor._node);
					costSoFar[neighbor._node] = newCost;
					cameFrom[neighbor._node] = current;
					frontier.push(neighbor._node, priority);
				}
			}
		}
	};
}

