#pragma once
#include <queue>
#include "node.h"

template<typename T, typename priorityValue>
struct PriorityQueue
{
	typedef std::pair<priorityValue, Node*> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priorityValue priority) { elements.emplace(priority, item); }

	T get()
	{
		T bestItem = elements.top().second;
		elements.pop();
		return bestItem;
	}
};

