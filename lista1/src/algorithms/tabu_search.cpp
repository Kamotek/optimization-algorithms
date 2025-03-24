//
// Created by kamil on 24.03.2025.
//

#include "tabu_search.h"
#include "astar.h"  // zawiera deklarację funkcji astar_change
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace chrono;

// Definicja operatora haszującego wektor stringów
size_t VectorHash::operator()(const vector<string>& v) const {
    hash<string> hasher;
    size_t seed = 0;
    for (const auto& s : v) {
        seed ^= hasher(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

// Generuje sąsiadów dla danego porządku przystanków poprzez zamianę par elementów
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

// Liczy liczbę przesiadek na trasie.
// Zmiana linii (porównywana za pomocą metody getLine()) traktowana jest jako przesiadka.
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

// Buduje pełną trasę, łącząc segmenty wyznaczone funkcją astar_change.
// Kolejne segmenty wyznaczane są między: start -> kolejny wymagany przystanek,
// aż do przystanku docelowego.
vector<edge> construct_route(const vector<string>& order,
                             const vector<edge>& edges,
                             const string& start,
                             const string& end,
                             const system_clock::time_point& startTime) {
    vector<edge> full_route;
    string current_stop = start;
    auto current_time = startTime;

    // Przechodzimy przez kolejne przystanki w zadanej kolejności
    for (const auto& next_stop : order) {
        auto segment = astar_change(edges, current_stop, next_stop, current_time);
        if (segment.empty()) return {}; // brak połączenia
        full_route.insert(full_route.end(), segment.begin(), segment.end());
        current_stop = next_stop;
        current_time = segment.back().getArrivalTime();
    }

    // Ostatni segment: z ostatniego przystanku wymaganego do przystanku docelowego
    auto final_segment = astar_change(edges, current_stop, end, current_time);
    if (final_segment.empty()) return {};
    full_route.insert(full_route.end(), final_segment.begin(), final_segment.end());

    return full_route;
}

// Oblicza koszt trasy jako liczbę przesiadek dla danego porządku odwiedzanych przystanków.
// Jeżeli trasy nie uda się zbudować, zwracamy INT_MAX.
int calculate_cost(const vector<string>& order,
                   const vector<edge>& edges,
                   const string& start,
                   const string& end,
                   const system_clock::time_point& startTime) {
    auto route = construct_route(order, edges, start, end, startTime);
    return route.empty() ? INT_MAX : count_transfers(route);
}

// Główna funkcja tabu_search, wyznaczająca trasę o minimalnej liczbie przesiadek.
// Jeśli lista required_stops jest pusta, funkcja wywołuje astar_change dla bezpośredniego połączenia.
vector<edge> tabu_search(const vector<edge>& edges,
                         const string& start,
                         const string& end,
                         const vector<string>& required_stops,
                         const system_clock::time_point& startTime,
                         int max_iterations) {
    // Jeżeli nie podano przystanków pośrednich, szukamy trasy bez zmian (astar_change)
    if (required_stops.empty()) {
        return astar_change(edges, start, end, startTime);
    }

    vector<string> current_order = required_stops;
    vector<string> best_order = current_order;
    vector<edge> best_route;
    int best_cost = calculate_cost(current_order, edges, start, end, startTime);

    unordered_set<size_t> tabu_list;
    VectorHash hash_fn;

    for (int iter = 0; iter < max_iterations; ++iter) {
        auto neighbors = generate_neighbors(current_order);
        int current_best_cost = INT_MAX;
        vector<string> current_best_order;

        // Przeglądamy wszystkich sąsiadów (różne permutacje kolejności przystanków)
        for (const auto& neighbor : neighbors) {
            size_t hash = hash_fn(neighbor);
            if (tabu_list.count(hash)) continue;

            int cost = calculate_cost(neighbor, edges, start, end, startTime);
            if (cost < current_best_cost) {
                current_best_cost = cost;
                current_best_order = neighbor;
            }
        }

        // Aktualizacja najlepszej znalezionej trasy, jeśli poprawiono koszt
        if (current_best_cost < best_cost) {
            best_cost = current_best_cost;
            best_order = current_best_order;
            best_route = construct_route(best_order, edges, start, end, startTime);
        }

        // Jeżeli nie znaleziono lepszej permutacji, przerywamy iteracje
        if (current_best_cost == INT_MAX) break;

        // Uaktualniamy bieżący porządek i dodajemy go do listy tabu
        current_order = current_best_order;
        tabu_list.insert(hash_fn(current_order));
    }

    return best_route;
}
