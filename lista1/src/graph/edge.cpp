//
// Created by kamil on 19.03.2025.
//

#include "edge.h"

edge::edge(uint32_t id,
           const std::string &company,
           const std::string &line,
           const time_point &departure_time,
           const time_point &arrival_time,
           const std::string &start_stop,
           const std::string &end_stop,
           double start_stop_lat,
           double start_stop_lon,
           double end_stop_lat,
           double end_stop_lon):
        id(id),
        company(company),
        line(line),
        departure_time(departure_time),
        arrival_time(arrival_time),
        start_stop(start_stop),
        end_stop(end_stop),
        start_stop_lat(start_stop_lat),
        start_stop_lon(start_stop_lon),
        end_stop_lat(end_stop_lat),
        end_stop_lon(end_stop_lon) {
}

#include <sstream>
#include <iomanip>   // Dodaj, jeśli jeszcze nie ma
#include <ctime>     // Potrzebne dla std::localtime

std::string edge::to_str() const {
    std::stringstream ss;
    // Konwersja time_point na std::time_t
    std::time_t arrival_time_t = std::chrono::system_clock::to_time_t(arrival_time);
    // Formatowanie daty i godziny (np. "YYYY-MM-DD HH:MM:SS")
    ss << "LINIA: " << line << " "
       << "GODZINA PRZYJAZDU: "
       << std::put_time(std::localtime(&arrival_time_t), "%F %T") << " "
       << start_stop << "-" << end_stop;
    return ss.str();
}
