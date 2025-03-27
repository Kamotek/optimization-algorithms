//
// Created by kamil on 23.03.2025.
//

#include "graph.h"

Graph::Graph() {

}

Graph::~Graph() {
}



void Graph::buildGraph(const std::vector<edge>& edges) {
    for (const auto &e : edges) {
        adj[e.getStartStop()].push_back(e);
    }
}


