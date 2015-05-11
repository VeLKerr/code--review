#ifndef COMPANY_H
#define COMPANY_H

#include <algorithm>
#include <vector>

/**
 *	I used vector<char> instead of vector<bool> because it works much faster.
 *  vector<bool> uses a bitmap compressing for storing its elements.
 */

typedef std::vector<std::vector<size_t> > AdjacencyList;

extern void DFS_directed(size_t vertex, const AdjacencyList &direct,
    std::vector<size_t> &order, std::vector<char> &used);

extern void DFS_inversed(size_t vertex, const AdjacencyList &inverse,
    std::vector<char> &component, std::vector<char> &used);

extern void update_min_compon_size(const std::vector<char> &component,
    const AdjacencyList &inverse, size_t &minComponentSize);

extern void read_conditions(AdjacencyList &direct, AdjacencyList &inverse);

#endif // !COMPANY_H
