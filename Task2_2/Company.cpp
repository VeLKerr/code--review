#include <iostream>
#include "Company.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

void DFS_directed(size_t vertex, const AdjacencyList &direct,
    vector<size_t> &order, vector<char> &used) {
    used[vertex] = true;
    for (size_t target: direct[vertex]) {
        if (!used[target]) {
            DFS_directed(target, direct, order, used);
        }
    }
    order.push_back(vertex);
}

void DFS_inversed(size_t vertex, const AdjacencyList &inverse,
    vector<char> &component, vector<char> &used) {
    used[vertex] = true;
    component[vertex] = true;
    for (size_t target : inverse[vertex]) {
        if (!used[target]) {
            DFS_inversed(target, inverse, component, used);
        }
    }
}

void update_min_compon_size(const vector<char> &component,
    const AdjacencyList &inverse, size_t &minComponentSize) {
    size_t componentSize = 0;
    for (size_t vertex = 0; vertex < component.size(); ++vertex) {
        if (component[vertex]) {
            for (size_t target: inverse[vertex]) {
                if (!component[target]) {
                    return;
                }
            }
            ++componentSize;
        }
    }
    minComponentSize = std::min(minComponentSize, componentSize);
}

void read_conditions(AdjacencyList &direct, AdjacencyList &inverse) {
    size_t vertexCnt, 
        edgeCnt;
    cin >> vertexCnt >> edgeCnt;
    direct.resize(vertexCnt);
    inverse.resize(vertexCnt);
    for (size_t i = 0; i < edgeCnt; ++i) {
        size_t source, 
            target, 
            weight;
        cin >> source >> target >> weight;
        if (weight == 1) {
            direct[source - 1].push_back(target - 1);
            inverse[target - 1].push_back(source - 1);
        } else if (weight == 2) {
            direct[target - 1].push_back(source - 1);
            inverse[source - 1].push_back(target - 1);
        }
    }
}

int main() {
    AdjacencyList direct, 
        inverse;
    read_conditions(direct, inverse);
    size_t vertexCnt = direct.size();
    vector<char> used(vertexCnt, false);
    vector<size_t> order;
    for (size_t i = 0; i < vertexCnt; ++i) {
        if (!used[i]) {
            DFS_directed(i, direct, order, used);
        }
    }
    used.assign(vertexCnt, false);
    size_t minComponentSize = vertexCnt;
    for (size_t i = 0; i < vertexCnt; ++i) {
        size_t vertex = order[vertexCnt - 1 - i];
        if (!used[vertex]) {
            vector<char> component(vertexCnt, false);
            DFS_inversed(vertex, inverse, component, used);
            update_min_compon_size(component, inverse, minComponentSize);
        }
    }
    cout << vertexCnt - (minComponentSize - 1) << endl;
}
