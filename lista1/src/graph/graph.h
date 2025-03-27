#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <unordered_map>
#include "vertex.h"
#include "edge.h"

class Graph {
private:
    std::unordered_map<std::string, std::vector<edge>> adj;
public:
    Graph();
    ~Graph();


    // Buduje graf na podstawie listy krawędzi.
    void buildGraph(const std::vector<edge>& edges);
    const std::unordered_map<std::string, std::vector<edge>>& getAdjacencyList() const {
        return adj;
    }

};

#endif // GRAPH_H
