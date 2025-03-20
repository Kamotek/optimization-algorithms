//
// Created by kamil on 19.03.2025.
//

#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H
#include <vector>

#include "graph.h"


class graph_generator {
private:
    std::vector<graph> graphs;

    static graph generate_graph(std::string row);
    public:
    graph_generator(const std::vector<std::string> &data_rows);
    std::vector<graph>get_graphs();
};

inline std::vector<graph> graph_generator::get_graphs() {
    return graphs;
}


#endif //GRAPH_GENERATOR_H
