//
// Created by kamil on 19.03.2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <chrono>
#include <string>


class graph {
    typedef std::chrono::system_clock::time_point time_point;
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

    //gettery
    uint32_t getId() const { return id; }
    const std::string& getCompany() const { return company; }
    const std::string& getLine() const { return line; }
    time_point getDepartureTime() const { return departure_time; }
    time_point getArrivalTime() const { return arrival_time; }
    const std::string& getStartStop() const { return start_stop; }
    const std::string& getEndStop() const { return end_stop; }
    double getStartStopLat() const { return start_stop_lat; }
    double getStartStopLon() const { return start_stop_lon; }
    double getEndStopLat() const { return end_stop_lat; }
    double getEndStopLon() const { return end_stop_lon; }

    // Settery
    void setId(uint32_t newId) { id = newId; }
    void setCompany(const std::string& newCompany) { company = newCompany; }
    void setLine(const std::string& newLine) { line = newLine; }
    void setDepartureTime(time_point newTime) { departure_time = newTime; }
    void setArrivalTime(time_point newTime) { arrival_time = newTime; }
    void setStartStop(const std::string& newStop) { start_stop = newStop; }
    void setEndStop(const std::string& newStop) { end_stop = newStop; }
    void setStartStopLat(double newLat) { start_stop_lat = newLat; }
    void setStartStopLon(double newLon) { start_stop_lon = newLon; }
    void setEndStopLat(double newLat) { end_stop_lat = newLat; }
    void setEndStopLon(double newLon) { end_stop_lon = newLon; }

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
