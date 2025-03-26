#include "vertex.h"
#include <utility>

Vertex::Vertex(std::string name) : stopName(std::move(name)) {}

const std::string& Vertex::getName() const {
    return stopName;
}

const std::vector<Vertex*>& Vertex::getNeighbors() const {
    return neighbors;
}

const std::vector<edge*>& Vertex::getEdges() const {
    return edges;
}

void Vertex::addNeighbor(Vertex* neighbor) {
    if (std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end()) {
        neighbors.push_back(neighbor);
    }
}

void Vertex::addEdge(edge* edgePtr) {
    if (std::find(edges.begin(), edges.end(), edgePtr) == edges.end()) {
        edges.push_back(edgePtr);
    }
}
