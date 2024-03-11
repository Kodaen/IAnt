#pragma once
#include <set>
#include <functional>
#include <map>
#include <queue>
#include <vector>
#include <climits>

#include "Node.h"
#include "Bug.h"

#define NULL_LOCATION Location(-1, -1)
#define INVALID_PATH_COST INT_MAX

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

		//Return the node with the lowest priority
		static bool CompareNodesPriority(Node<T>* a, Node<T>* b)
		{
			return a->_priority > b->_priority;
		}

		PathData<T> findPath(T from, T to, std::function<float(T, T)> heuristic)
		{
			if (!_nodes.count(from)) return PathData<T>(false);
			if (!_nodes.count(to)) return PathData<T>(false);
			Node<T>* departureNode = _nodes[from];
			Node<T>* destinationNode = _nodes[to];
			std::priority_queue<Node<T>*, std::vector<Node<T>*>, std::function<bool(Node<T>*, Node<T>*)>> frontier(CompareNodesPriority);
			frontier.push(departureNode);
			std::map<T, T> cameFrom;
			cameFrom[from] = NULL_LOCATION;
			std::map<T, float> costSoFar;
			costSoFar[from] = 0;
			bool pathFound = false;

			while (!frontier.empty())
			{
				Node<T>* current = frontier.top();
				frontier.pop();

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

					neighbor._node->_priority = (newCost + heuristic(destinationNode->getData(), neighbor._node->getData()));
					frontier.push(neighbor._node);
				}
				if (pathFound) break;
			}

			vector<T> reversePath;
			reversePath.push_back(to);
			auto previousLocation = cameFrom[to];
			while (previousLocation != from) {
				reversePath.push_back(previousLocation);
				previousLocation = cameFrom[previousLocation];
			}

			return PathData<T>(reversePath, costSoFar[to]);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="origin">The "center" node, the one we start from</param>
		/// <param name="minCost">A node must have a cost equal or higher to this value to be in range</param>
		/// <param name="maxCost">A node must have a cost equal or lesser to this value to be in range</param>
		/// <param name="shouldBeInSample">If true, only data nodes in a sample will be returned (and count in the stop after feature)</param>
		/// <param name="sample">If shouldBeInSample is true, only node in this sample will be returned (and cound in the stop after feature)</param>
		/// <param name="shouldStopAfter">Set to true if you want to stop once you found a certain number of nodes</param>
		/// <param name="stopAfter">The number of node you require before stopping, if should stop after is set to true</param>
		/// <returns></returns>
		std::vector<T> findDataOfNodesBetween(T origin, float minCost, float maxCost, bool shouldBeInSample = false, std::vector<T> sample = {}, bool shouldStopAfter = false, int stopAfter = 0)
		{
			if (!_nodes.count(origin)) return std::vector<T>();

			std::vector<Node<T>*> frontier;
			frontier.push_back(_nodes[origin]);
			std::map<T, float> costSoFar;
			costSoFar[origin] = 0;

			std::vector<T> nodesInRange;

			while (!frontier.empty())
			{
				Node<T>* current = frontier.back();
				frontier.pop_back();

				//Iterate over graph and add each node in range to the vecotr
				for each (Neighbor<T> neighbor in current->getNeighbors())
				{
					if (neighbor._node == current) continue;
					float newCost = costSoFar[current->getData()] + neighbor._costToReach;

					//If the path to the evaluated node is more costly than the max cost, we ignore it before doing anything else, ensuring that the frontier will stay empty once every node has been found
					if (newCost > maxCost) continue;
					//If the path to the evaluated node is either non existent or more costly, we register the new path
					if (costSoFar.count(neighbor._node->getData()) && costSoFar[neighbor._node->getData()] <= newCost) continue;

					bool costTooLow = (newCost < minCost);
					//We should also check that the node that is now too close wasn't previously on the list
					if (costTooLow && costSoFar.count(neighbor._node->getData()))
					{
						//If so, remove the node now that we know it's too close
						nodesInRange.erase(std::remove(nodesInRange.begin(), nodesInRange.end(), neighbor._node->getData()), nodesInRange.end());
					}

					costSoFar[neighbor._node->getData()] = newCost;
					frontier.push_back(neighbor._node);

					if (costTooLow) continue;
					if (count(nodesInRange.begin(), nodesInRange.end(), neighbor._node->getData())) continue;
					if (shouldBeInSample && !count(sample.begin(), sample.end(), neighbor._node->getData())) continue;

					nodesInRange.push_back(neighbor._node->getData());

					if (shouldStopAfter && nodesInRange.size() >= stopAfter) return nodesInRange;
				}
			}
			return nodesInRange;
		}
	};
}

