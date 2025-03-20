//
// Created by kamil on 19.03.2025.
//

#include "graph_generator.h"

#include "graph.h"


std::chrono::high_resolution_clock::time_point parse_time(const std::string &time_str) {
    std::tm tm = {};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, "%H:%M:%S");
    std::time_t tt = std::mktime(&tm);
    return std::chrono::high_resolution_clock::from_time_t(tt);
}

graph graph_generator::generate_graph(std::string row) {
    std::stringstream ss(row);
    char delimiter = ',';

    std::string token;
    std::vector<std::string> splitted;
    while (getline(ss, token, delimiter)) {
        splitted.push_back(token);
    }

    uint32_t id = static_cast<uint32_t>(std::stoul(splitted[0]));
    std::string company = splitted[1];
    std::string line = splitted[2];
    auto departure_time = parse_time(splitted[3]);
    auto arrival_time = parse_time(splitted[4]);
    std::string start_stop = splitted[5];
    std::string end_stop = splitted[6];
    double start_stop_lat = std::stod(splitted[7]);
    double start_stop_lon = std::stod(splitted[8]);
    double end_stop_lat = std::stod(splitted[9]);
    double end_stop_lon = std::stod(splitted[10]);

    return graph(id, company, line, departure_time, arrival_time,
                 start_stop, end_stop, start_stop_lat, start_stop_lon,
                 end_stop_lat, end_stop_lon);
}

graph_generator::graph_generator(const std::vector<std::string> &data_rows) {
    for (auto row : data_rows) {
        graphs.push_back(generate_graph(row));
    }
}


