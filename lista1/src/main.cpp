#include <iostream>
#include <vector>

#include "algorithms/dijkstra.h"
#include "io_handling//csv_reader.h"
#include "graph/edge.h"
#include "graph/graph_generator.h"
#include "ui/user_cli.h"
#include "algorithms_utils/time_calc.h"
#include "graph/graph.h"
#include "algorithms/dijkstra.h"
#include "algorithms/astar.h"
#include "algorithms/tabu_search.h"

int main() {




    std::vector<std::string> data = readCSVFile("../data/connection_graph.csv");
    data.erase(data.begin());


    graph_generator generator = graph_generator(data);

    std::vector<edge> edges = generator.get_graphs();

    // Budujemy graf
    Graph graph;
    graph.buildGraph(edges);



    auto startTime = std::chrono::system_clock::now()+std::chrono::hours(12);


    std::vector<edge> bestRoute1 = dijkstra_time(edges, "BISKUPIN", "KOZANÓW", startTime);
    std::vector<edge> bestRoute2 = astar_time(edges, "BISKUPIN", "KOZANÓW", startTime);
    std::vector<edge> bestRoute3 = astar_change(edges, "BISKUPIN", "KOZANÓW", startTime);

    std::vector<edge> tabu_route = tabu_search(edges, "BISKUPIN", "KOZANÓW",{"KROMERA", "Reja", "Zamkowa", "Reja"}, startTime);


     for (const auto& edge : bestRoute1) {
         std::cout << edge.to_str() << std::endl;
     }

    std::cout << "===================" << std::endl;

    for (const auto& edge : bestRoute2) {
        std::cout << edge.to_str() << std::endl;
    }

    std::cout << "====================" << std::endl;

    for (const auto& edge : bestRoute3) {
        std::cout << edge.to_str() << std::endl;
    }

    std::cout << "====================" << std::endl;

    for (const auto& edge : tabu_route) {
        std::cout << edge.to_str() << std::endl;
    }


    return 0;
}
