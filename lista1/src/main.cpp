#include <iostream>
#include <vector>

#include "graph_utils/csv_reader.h"
#include "graph_utils/graph.h"
#include "graph_utils/graph_generator.h"

int main() {
    std::vector<std::string> data = readCSVFile("../data/connection_graph.csv");
    data.erase(data.begin());


    graph_generator generator = graph_generator(data);

    std::vector<graph> graphs = generator.get_graphs();

    for (graph graph : graphs) {
        std::cout << graph.to_str() << std::endl;
    }


    return 0;
}
