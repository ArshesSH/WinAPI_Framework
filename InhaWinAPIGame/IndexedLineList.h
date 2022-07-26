#pragma once

#include <vector>

template <typename V>
struct IndexedLineList
{
	std::vector<V> vertices;
	std::vector<size_t> indices;
};