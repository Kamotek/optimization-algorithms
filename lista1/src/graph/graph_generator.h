//
// Created by kamil on 19.03.2025.
//

#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H
#include <vector>

#include "edge.h"


class graph_generator {
private:
    std::vector<edge> graphs;

    static edge generate_graph(std::string row);
    public:
    graph_generator(const std::vector<std::string> &data_rows);
    std::vector<edge>get_graphs();
};

inline std::vector<edge> graph_generator::get_graphs() {
    return graphs;
}


#endif //GRAPH_GENERATOR_H
