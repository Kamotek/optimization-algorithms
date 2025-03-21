#include <iostream>
#include <vector>

#include "graph_utils/csv_reader.h"
#include "graph_utils/graph.h"
#include "graph_utils/graph_generator.h"
#include "ui/user_cli.h"

int main() {
    std::vector<std::string> data = readCSVFile("../data/connection_graph.csv");
    data.erase(data.begin());


    graph_generator generator = graph_generator(data);

    std::vector<graph> graphs = generator.get_graphs();

     for (int i = 0; i < 3; ++i) {
         std::cout << graphs[i].to_str() << std::endl;
         std::cout << graphs[i].getArrivalTime()-graphs[i].getDepartureTime() << std::endl;
     }

    user_cli user_cli;

    std::cout<<user_cli.get_end_stop()<<std::endl;
    std::cout<<user_cli.get_start_stop()<<std::endl;
    std::cout<<user_cli.get_optimization_criteria()<<std::endl;
    std::cout<<user_cli.get_start_time()<<std::endl;


    return 0;
}
