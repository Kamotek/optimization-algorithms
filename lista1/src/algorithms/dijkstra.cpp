//
// Created by kamil on 23.03.2025.
//

#include "dijkstra.h"

bool State::operator>(const State& other) const {
    return time > (other.time);
}

std::vector<edge> dijkstra_time(const std::vector<edge>& edges,
                                const std::string& start,
                                const std::string& end,
                                const std::chrono::system_clock::time_point& startTime) {
    // lista sąsiedztwa
    std::unordered_map<std::string, std::vector<edge>> adj;
    for (const auto &e : edges) {
        adj[e.getStartStop()].push_back(e);
    }

    // najlepszy czas na danym przystanku
    std::unordered_map<std::string, std::chrono::system_clock::time_point> best;
    best[start] = startTime;

    // priority queue
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({start, startTime, {}});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        // zwracamy trase
        if (current.stop == end) {
            return current.route;
        }

        // Jeśli brak połączeń wychodzących z bieżącego przystanku, przechodzimy dalej.
        if (adj.find(current.stop) == adj.end()) {
            continue;
        }

        // Przeglądamy wszystkie możliwe połączenia z aktualnego przystanku.
        for (const auto &e : adj[current.stop]) {
            // Wsiadamy tylko, gdy czas odjazdu nie jest wcześniejszy niż aktualny czas.
            if (e.getDepartureTime() >= current.time) {
                auto arrival = e.getArrivalTime();
                // Jeśli znaleziono lepszy czas przyjazdu dla przystanku docelowego tej krawędzi.
                if (best.find(e.getEndStop()) == best.end() || arrival < best[e.getEndStop()]) {
                    best[e.getEndStop()] = arrival;
                    std::vector<edge> newRoute = current.route;
                    newRoute.push_back(e);
                    pq.push({e.getEndStop(), arrival, newRoute});
                }
            }
        }
    }

    return {};
}


