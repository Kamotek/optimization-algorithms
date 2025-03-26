#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <unordered_map>
#include "vertex.h"
#include "edge.h"

class Graph {
private:
    std::unordered_map<std::string, Vertex*> nodes;
public:
    Graph();
    ~Graph();

    // Dodaje krawędź do grafu, generując unikalne wierzchołki.
    void addEdge(const edge &e);

    // Buduje graf na podstawie listy krawędzi.
    void buildGraph(const std::vector<edge>& edges);

    // Zwraca wektor wskaźników do wierzchołków grafu.
    std::vector<Vertex*> getNodes() const;
};

#endif // GRAPH_H
