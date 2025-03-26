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

std::vector<edge> dijkstra_change(const std::vector<edge>& edges,
                                  const std::string& start,
                                  const std::string& end,
                                  const std::chrono::system_clock::time_point& startTime) {
    // Tworzymy listę sąsiedztwa
    std::unordered_map<std::string, std::vector<edge>> adj;
    for (const auto &e : edges) {
        adj[e.getStartStop()].push_back(e);
    }


    // Struktura przechowująca najlepszą (minimalną) liczbę przesiadek dla danego przystanku i linii
    std::unordered_map<std::string, std::unordered_map<std::string, int>> best;

    // Kolejka priorytetowa, w której priorytet ma mniejsza liczba przesiadek
    std::priority_queue<StateChange, std::vector<StateChange>, std::greater<StateChange>> pq;
    // Początkowy stan: startowy przystanek, startTime, 0 przesiadek, brak wybranej linii
    pq.push({start, startTime, 0, "", {}});
    best[start][""] = 0;

    while (!pq.empty()) {
        StateChange current = pq.top();
        pq.pop();

        // Jeśli dotarliśmy do celu, zwracamy trasę
        if (current.stop == end) {
            return current.route;
        }

        // Jeśli dla bieżącego przystanku brak sąsiadów, przechodzimy dalej.
        if (adj.find(current.stop) == adj.end()) {
            continue;
        }

        // Sprawdzamy wszystkie połączenia wychodzące z aktualnego przystanku.
        for (const auto &e : adj[current.stop]) {
            // Wsiadamy tylko, gdy czas odjazdu nie jest wcześniejszy niż bieżący czas.
            if (e.getDepartureTime() >= current.time) {
                int new_transfers = current.transfers;
                std::string next_line = e.getLine();

                // Jeśli nie jest to pierwszy odcinek i linia zmienia się, zwiększamy licznik przesiadek.
                if (!current.current_line.empty() && current.current_line != next_line) {
                    new_transfers++;
                }

                auto arrival = e.getArrivalTime();
                // Sprawdzamy, czy udało się uzyskać lepszy (mniejszy) stan dla przystanku docelowego przy danej linii.
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
    // Jeśli nie znaleziono trasy, zwracamy pusty wektor.
    return {};
}



