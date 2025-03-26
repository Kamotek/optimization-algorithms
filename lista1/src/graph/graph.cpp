//
// Created by kamil on 23.03.2025.
//

#include "graph.h"

Graph::Graph() {

}

Graph::~Graph() {
    for (auto pair : nodes) {
        delete pair.second;
    }
}

void Graph::addEdge(const edge &e) {
    Vertex* startNode = nullptr;
    Vertex* endNode = nullptr;

    // Sprawdzenie, czy wierzchołek początkowy już istnieje
    if (nodes.find(e.getStartStop()) == nodes.end()) {
        startNode = new Vertex(e.getStartStop());
        nodes[e.getStartStop()] = startNode;
    } else {
        startNode = nodes[e.getStartStop()];
    }

    // Sprawdzenie, czy wierzchołek końcowy już istnieje
    if (nodes.find(e.getEndStop()) == nodes.end()) {
        endNode = new Vertex(e.getEndStop());
        nodes[e.getEndStop()] = endNode;
    } else {
        endNode = nodes[e.getEndStop()];
    }

    // Dodanie wierzchołka końcowego jako sąsiada wierzchołka początkowego
    startNode->addNeighbor(endNode);
}

void Graph::buildGraph(const std::vector<edge>& edges) {
    for (const auto &e : edges) {
        addEdge(e);
    }
}

std::vector<Vertex*> Graph::getNodes() const {
    std::vector<Vertex*> result;
    for (auto const& pair : nodes) {
        result.push_back(pair.second);
    }
    return result;
}
