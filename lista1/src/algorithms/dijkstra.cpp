//
// Created by kamil on 23.03.2025.
//

#include "dijkstra.h"

bool State::operator>(const State& other) const {
    return time > (other.time);
}

bool StateChange::operator>(const StateChange &other) const {
    return transfers > other.transfers;
}


std::pair<std::vector<edge>, double> dijkstra_time(const std::vector<edge>& edges,
                                                   const std::string& start,
                                                   const std::string& end,
                                                   const std::chrono::system_clock::time_point& startTime) {
    std::unordered_map<std::string, std::vector<edge>> adj;
    for (const auto &e : edges) {
        adj[e.getStartStop()].push_back(e);
    }

    std::unordered_map<std::string, std::chrono::system_clock::time_point> best;
    best[start] = startTime;

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({start, startTime, {}});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.stop == end) {
            // Obliczamy funkcję kosztu: różnica czasu w sekundach
            double cost = std::chrono::duration_cast<std::chrono::seconds>(current.time - startTime).count();
            return {current.route, cost};
        }

        if (adj.find(current.stop) == adj.end()) {
            continue;
        }

        for (const auto &e : adj[current.stop]) {
            if (e.getDepartureTime() >= current.time) {
                auto arrival = e.getArrivalTime();
                if (best.find(e.getEndStop()) == best.end() || arrival < best[e.getEndStop()]) {
                    best[e.getEndStop()] = arrival;
                    std::vector<edge> newRoute = current.route;
                    newRoute.push_back(e);
                    pq.push({e.getEndStop(), arrival, newRoute});
                }
            }
        }
    }
    // Jeśli nie znaleziono trasy, zwracamy pustą trasę oraz koszt równy -1 (lub inną wartość sygnalizującą błąd)
    return {{}, -1.0};
}

std::pair<std::vector<edge>, double> dijkstra_change(const std::vector<edge>& edges,
                                                  const std::string& start,
                                                  const std::string& end,
                                                  const std::chrono::system_clock::time_point& startTime) {
    std::unordered_map<std::string, std::vector<edge>> adj;
    for (const auto &e : edges) {
        adj[e.getStartStop()].push_back(e);
    }

    std::unordered_map<std::string, std::unordered_map<std::string, int>> best;
    std::priority_queue<StateChange, std::vector<StateChange>, std::greater<StateChange>> pq;
    pq.push({start, startTime, 0, "", {}});
    best[start][""] = 0;

    while (!pq.empty()) {
        StateChange current = pq.top();
        pq.pop();

        if (current.stop == end) {
            return {current.route, current.transfers};
        }

        if (adj.find(current.stop) == adj.end()) {
            continue;
        }

        for (const auto &e : adj[current.stop]) {
            if (e.getDepartureTime() >= current.time) {
                int new_transfers = current.transfers;
                std::string next_line = e.getLine();

                if (!current.current_line.empty() && current.current_line != next_line) {
                    new_transfers++;
                }

                auto arrival = e.getArrivalTime();
                if (best[e.getEndStop()].find(next_line) == best[e.getEndStop()].end() ||
                    new_transfers < best[e.getEndStop()][next_line]) {
                    best[e.getEndStop()][next_line] = new_transfers;
                    std::vector<edge> newRoute = current.route;
                    newRoute.push_back(e);
                    pq.push({e.getEndStop(), arrival, new_transfers, next_line, newRoute});
                }
            }
        }
    }
    // Jeśli nie znaleziono trasy, zwracamy pustą trasę oraz koszt równy -1 (lub inną wartość sygnalizującą błąd)
    return {{}, -1.0};
}



