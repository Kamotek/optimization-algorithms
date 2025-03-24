//
// Created by kamil on 23.03.2025.
//

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
    ~Graph() {
        // Zwolnienie pamięci zaalokowanej dla węzłów
        for (auto pair : nodes) {
            delete pair.second;
        }
    }

    // Metoda dodająca krawędź do grafu, tworzy nowe węzły jeśli nie istnieją
    void addEdge(const edge &e) {
        Vertex* startNode = nullptr;
        Vertex* endNode = nullptr;

        // Sprawdzamy czy węzeł startowy już istnieje
        if (nodes.find(e.getStartStop()) == nodes.end()) {
            startNode = new Vertex(e.getStartStop());
            nodes[e.getStartStop()] = startNode;
        } else {
            startNode = nodes[e.getStartStop()];
        }

        // Sprawdzamy czy węzeł końcowy już istnieje
        if (nodes.find(e.getEndStop()) == nodes.end()) {
            endNode = new Vertex(e.getEndStop());
            nodes[e.getEndStop()] = endNode;
        } else {
            endNode = nodes[e.getEndStop()];
        }

        // Dodajemy sąsiedztwo – zakładamy graf skierowany
        startNode->addNeighbor(endNode);
    }

    // Metoda budująca graf na podstawie listy krawędzi
    void buildGraph(const std::vector<edge>& edges) {
        for (const auto &e : edges) {
            addEdge(e);
        }
    }

    // Metoda zwracająca listę węzłów (przystanków)
    std::vector<Vertex*> getNodes() const {
        std::vector<Vertex*> result;
        for (auto const& pair : nodes) {
            result.push_back(pair.second);
        }
        return result;
    }
};

#endif // GRAPH_H
