/**
 * @file tabu_search.cpp
 * @brief Implementacja algorytmu Tabu Search wraz z funkcjami pomocniczymi.
 *
 * Ten plik zawiera implementację funkcji wspomagających dla algorytmu Tabu Search,
 * m.in. funkcje generujące sąsiadów, liczące przesiadki oraz budujące trasę, a także
 * główną funkcję tabu_search.
 */

#include "tabu_search.h"
#include "astar.h"  // Zawiera deklarację funkcji astar_change
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <deque>
#include <queue>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace chrono;


size_t VectorHash::operator()(const vector<string>& v) const {
    hash<string> hasher;
    size_t seed = 0;
    for (const auto& s : v) {
        seed ^= hasher(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}


vector<vector<string>> generate_neighbors(const vector<string>& current) {
    vector<vector<string>> neighbors;
    for (size_t i = 0; i < current.size(); ++i) {
        for (size_t j = i + 1; j < current.size(); ++j) {
            vector<string> neighbor = current;
            swap(neighbor[i], neighbor[j]);
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}


int count_transfers(const vector<edge>& route) {
    if (route.empty()) return 0;
    int transfers = 0;
    string last_line = route[0].getLine();
    for (size_t i = 1; i < route.size(); ++i) {
        if (route[i].getLine() != last_line) {
            transfers++;
            last_line = route[i].getLine();
        }
    }
    return transfers;
}


vector<edge> construct_route(const vector<string>& order,
                             const vector<edge>& edges,
                             const string& start,
                             const string& end,
                             const system_clock::time_point& startTime) {
    vector<edge> full_route;
    string current_stop = start;
    auto current_time = startTime;

    // Iteracja po zadanej kolejności przystanków
    for (const auto& next_stop : order) {
        auto segment = astar_change(edges, current_stop, next_stop, current_time).first;
        if (segment.empty()) return {}; // Brak połączenia dla danego segmentu
        full_route.insert(full_route.end(), segment.begin(), segment.end());
        current_stop = next_stop;
        current_time = segment.back().getArrivalTime();
    }

    // Łączymy ostatni segment od ostatniego przystanku wymaganego do docelowego
    auto final_segment = astar_change(edges, current_stop, end, current_time).first;
    if (final_segment.empty()) return {};
    full_route.insert(full_route.end(), final_segment.begin(), final_segment.end());

    return full_route;
}


double calculate_cost(const vector<string>& order,
                      const vector<edge>& edges,
                      const string& start,
                      const string& end,
                      const system_clock::time_point& startTime) {
    auto route = construct_route(order, edges, start, end, startTime);
    return route.empty() ? INT_MAX : count_transfers(route);
}


pair<vector<edge>, double> tabu_search(const vector<edge>& edges,
                                         const string& start,
                                         const string& end,
                                         const vector<string>& required_stops,
                                         const system_clock::time_point& startTime,
                                         int max_iterations) {
    // Jeżeli nie podano przystanków pośrednich, szukamy bezpośredniej trasy (astar_change)
    if (required_stops.empty()) {
        auto route = astar_change(edges, start, end, startTime).first;
        double cost = route.empty() ? INT_MAX : count_transfers(route);
        return {route, cost};
    }

    // Inicjalizacja bieżącego porządku i globalnego najlepszego rozwiązania
    vector<string> current_order = required_stops;
    vector<string> best_order = current_order;
    vector<edge> best_route = construct_route(best_order, edges, start, end, startTime);
    double best_cost = best_route.empty() ? INT_MAX : count_transfers(best_route);

    VectorHash hash_fn;
    // Implementacja listy tabu jako kolejki FIFO
    deque<size_t> tabu_queue;
    unordered_set<size_t> tabu_set;
    // Rozmiar listy tabu zależy od liczby przystanków – ustawiony na 2 * liczba przystanków
    size_t tabu_max_size = 2 * required_stops.size();

    for (int iter = 0; iter < max_iterations; ++iter) {
        auto neighbors = generate_neighbors(current_order);
        double current_best_cost = INT_MAX;
        vector<string> current_best_order;

        // Przeglądanie wszystkich sąsiadów (pełna permutacja)
        for (const auto& neighbor : neighbors) {
            size_t neighbor_hash = hash_fn(neighbor);
            int cost = calculate_cost(neighbor, edges, start, end, startTime);

            // Warunek aspiracji: jeśli sąsiad poprawia globalny wynik, wybieramy go natychmiast
            if (cost < best_cost) {
                current_best_cost = cost;
                current_best_order = neighbor;
                break;
            }

            // Pomijamy rozwiązania, które znajdują się na liście tabu
            if (tabu_set.find(neighbor_hash) != tabu_set.end()) continue;

            if (cost < current_best_cost) {
                current_best_cost = cost;
                current_best_order = neighbor;
            }
        }

        // Jeśli nie znaleziono żadnego poprawiającego sąsiada, kończymy iteracje
        if (current_best_cost == INT_MAX) break;

        // Aktualizacja globalnie najlepszego rozwiązania
        if (current_best_cost < best_cost) {
            best_cost = current_best_cost;
            best_order = current_best_order;
            best_route = construct_route(best_order, edges, start, end, startTime);
        }

        // Aktualizacja listy tabu – dodajemy bieżący porządek i usuwamy najstarszy, jeśli przekroczono limit
        size_t current_hash = hash_fn(current_order);
        tabu_queue.push_back(current_hash);
        tabu_set.insert(current_hash);
        if (tabu_queue.size() > tabu_max_size) {
            size_t old_hash = tabu_queue.front();
            tabu_queue.pop_front();
            tabu_set.erase(old_hash);
        }

        // Uaktualniamy bieżący porządek do najlepszego znalezionego rozwiązania w tej iteracji
        current_order = current_best_order;
    }

    return {best_route, best_cost};
}
