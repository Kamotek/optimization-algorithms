//
// Created by kamil on 19.03.2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <chrono>
#include <string>


class graph {
    typedef std::chrono::high_resolution_clock::time_point time_point;
public:
    graph(uint32_t id,
        const std::string &company,
        const std::string &line,
        const time_point &departure_time,
        const time_point &arrival_time,
        const std::string &start_stop,
        const std::string &end_stop,
        double start_stop_lat,
        double start_stop_lon,
        double end_stop_lat,
        double end_stop_lon);

    std::string to_str();
private:
  //  4,MPK Autobusy,A,20:57:00,20:59:00,Bałtycka,Broniewskiego,51.13663187,17.0306168,51.13585101,17.03738306
    std::uint32_t id;
    std::string company;
    std::string line;
    time_point departure_time;
    time_point arrival_time;
    std::string start_stop;
    std::string end_stop;
    double start_stop_lat;
    double start_stop_lon;
    double end_stop_lat;
    double end_stop_lon;

};



#endif //GRAPH_H
