//
// Created by kamil on 21.03.2025.
//

#ifndef USER_CLI_H
#define USER_CLI_H

#include <chrono>
#include <string>

class user_cli {
private:
    std::string start_stop;
    std::string end_stop;
    char optimization_criteria{};
    std::chrono::high_resolution_clock::time_point start_time;

public:
    user_cli();
    std::string get_start_stop();
    std::string get_end_stop();
    char get_optimization_criteria();
    std::chrono::high_resolution_clock::time_point get_start_time();
    void set_start_stop(std::string start_stop);
    void set_end_stop(std::string end_stop);
    void set_optimization_criteria(char optimization_criteria);
    void set_start_time(std::chrono::high_resolution_clock::time_point start_time);
};

#endif //USER_CLI_H
