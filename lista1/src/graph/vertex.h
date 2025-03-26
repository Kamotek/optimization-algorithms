#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>
#include <algorithm>
#include "edge.h"

class Vertex {
private:
    std::string stopName;
    std::vector<Vertex*> neighbors;
    std::vector<edge*> edges;
public:
    explicit Vertex(std::string name);

    // Gettery
    const std::string& getName() const;
    const std::vector<Vertex*>& getNeighbors() const;
    const std::vector<edge*>& getEdges() const;

    // Metody dodające sąsiadów i krawędzie
    void addNeighbor(Vertex* neighbor);
    void addEdge(edge* edgePtr);
};

#endif // VERTEX_H
