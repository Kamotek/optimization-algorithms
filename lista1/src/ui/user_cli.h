//
// Created by kamil on 21.03.2025.
//

#ifndef USER_CLI_H
#define USER_CLI_H

// user_cli.h
#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "../graph/edge.h"
#ifdef _WIN32
#include <windows.h>
#endif

class user_cli {
public:
    user_cli();
std::pair<std::vector<edge>,double> execute(const std::vector<edge>& edges);

    std::string get_start_stop() const;
    std::string get_end_stop() const;
    char get_optimization_criteria() const;
    std::chrono::system_clock::time_point get_start_time() const;
    int get_algorithm_choice() const;
    std::vector<std::string> get_excluded_stops() const;

private:
    void gatherBasicData();
    void gatherAlgorithmChoice();

    std::string start_stop;
    std::string end_stop;
    char optimization_criteria; // 't' - czasowe, 'p' - przesiadkowe
    std::chrono::system_clock::time_point start_time;
    int algorithm_choice; // 1 - Dijkstra, 2 - A*, 3 - Tabu Search
    std::vector<std::string> excluded_stops;
};

#endif //USER_CLI_H
