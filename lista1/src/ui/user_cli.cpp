//
// Created by kamil on 21.03.2025.
//

#include "user_cli.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <utility>

user_cli::user_cli() {
    std::cout << "Podaj przystanek początkowy: " << std::endl;
    std::string temp;
    std::cin >> temp;
    set_start_stop(temp);

    std::cout << "Podaj przystanek końcowy: " << std::endl;
    std::string temp2;
    std::cin >> temp2;
    set_end_stop(temp2);

    std::cout << "Podaj czas (HH:MM:SS): " << std::endl;
    std::string time_str;
    std::cin >> time_str;

    int hh, mm, ss;
    char colon1, colon2;
    std::istringstream iss(time_str);
    if (iss >> hh >> colon1 >> mm >> colon2 >> ss && colon1 == ':' && colon2 == ':') {
        // Tworzymy czas jako sumę godzin, minut i sekund
        auto duration_since_midnight = std::chrono::hours(hh)
                                     + std::chrono::minutes(mm)
                                     + std::chrono::seconds(ss);
        // Tworzymy time_point, traktując epokę jako 0:00:00
        std::chrono::high_resolution_clock::time_point tp(duration_since_midnight);
        set_start_time(tp);
    } else {
        std::cerr << "Niepoprawny format czasu." << std::endl;
        //set_start_time(std::chrono::high_resolution_clock::now());
    }

    std::cout << "Wybierz kryterium (t - najkrótszy dojazd, p - najmniej przesiadek): " << std::endl;
    char criteria;
    std::cin >> criteria;
    set_optimization_criteria(criteria);
}

std::string user_cli::get_start_stop() {
    return start_stop;
}

std::string user_cli::get_end_stop() {
    return end_stop;
}

char user_cli::get_optimization_criteria() {
    return optimization_criteria;
}

std::chrono::high_resolution_clock::time_point user_cli::get_start_time() {
    return start_time;
}

void user_cli::set_start_stop(std::string start_stop) {
    this->start_stop = std::move(start_stop);
}

void user_cli::set_end_stop(std::string end_stop) {
    this->end_stop = std::move(end_stop);
}

void user_cli::set_optimization_criteria(char optimization_criteria) {
    this->optimization_criteria = optimization_criteria;
}

void user_cli::set_start_time(std::chrono::high_resolution_clock::time_point start_time) {
    this->start_time = start_time;
}
