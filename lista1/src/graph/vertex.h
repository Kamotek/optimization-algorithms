//
// Created by kamil on 23.03.2025.
//

#ifndef NODE_H
#define NODE_H

#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include "edge.h"

class Vertex {
private:
    std::string stopName;
    std::vector<Vertex*> neighbors;
    std::vector<edge*> edges;
public:
    explicit Vertex(std::string name) : stopName(std::move(name)) {}

    [[nodiscard]] const std::string& getName() const { return stopName; }
    [[nodiscard]] const std::vector<Vertex*>& getNeighbors() const { return neighbors; }
    [[nodiscard]] const std::vector<edge*>& getEdges() const { return edges; }

    void addNeighbor(Vertex* neighbor) {
        if(std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end()) {
            neighbors.push_back(neighbor);
        }
    }
    void addEdge(edge* edge) {
        if(std::find(edges.begin(), edges.end(), edge) == edges.end()) {
            edges.push_back(edge);
        }
    }

};

#endif // NODE_H
