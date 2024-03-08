#pragma once
#include <vector>

namespace Astar
{
	template <class T>
	class Node;

	template<class T>
	struct Neighbor 
	{
		Node<T>* _node;
		float _costToReach;
	};

	/// <summary>
	/// Represents a Node in a graph
	/// The node can correspond to any kind of data in your game: a Location, a City... any class that you want
	/// This .h contains a lot of code, you may be tempted to move it to a cpp, but you can't, because Template's functions can't really do that as it tends to cause Linker errors
	/// https://stackoverflow.com/questions/1639797/template-issue-causes-linker-error-c
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class Node
	{
	private:
		std::vector<Neighbor<T>> _neighbors = std::vector<Neighbor<T>>();
		T _data;
	public:
		float _priority = 0;

		Node() {}
		~Node() {}

		void addNeighbor(Node* _node, const float costToReach)
		{
			addNeighbor(Neighbor<T>{ _node, costToReach });
		}

		bool hasNeighbor(const Node<T> _node) const
		{
			for each (Neighbor<T> neighbor in _neighbors)
			{
				if (*neighbor._node == _node)
				{
					return true;
				}
			}
			return false;
		}

		std::vector<Neighbor<T>> getNeighbors()
		{
			return _neighbors;
		}

		Node(T data, std::vector<Neighbor<T>> neighbors)
		{
			_data = data;
			for each (auto n in neighbors)
			{
				addNeighbor(n);
			}
		}
		
		void addNeighbor(Neighbor<T> neighbor)
		{
			if (hasNeighbor(*neighbor._node)) return;
			_neighbors.push_back(neighbor);
		}

		T getData()
		{
			return _data;
		}

		bool operator==(const Node<T>& n2) const { return this->_data==n2._data; }
	
		friend std::ostream& operator<<(std::ostream& os, const Node<T>& node)
		{
			os <<"Priority "<<node._priority<<" | Data \""<< node._data<<"\"";
			return os;
		}
	};
}
