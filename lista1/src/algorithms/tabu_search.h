//
// Created by kamil on 24.03.2025.
//

#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include <vector>
#include <string>
#include <chrono>
#include <climits>
#include <unordered_set>
#include "../graph/edge.h"
#include "astar.h"

// Struktura haszująca wektor stringów
struct VectorHash {
    size_t operator()(const std::vector<std::string>& v) const;
};

// Generuje sąsiadów dla danego porządku przystanków
std::vector<std::vector<std::string>> generate_neighbors(const std::vector<std::string>& current);

// Liczy liczbę przesiadek w trasie (zmiana linii traktowana jako przesiadka)
int count_transfers(const std::vector<edge>& route);

// Buduje pełną trasę na podstawie kolejności odwiedzanych przystanków
std::vector<edge> construct_route(const std::vector<std::string>& order,
                                  const std::vector<edge>& edges,
                                  const std::string& start,
                                  const std::string& end,
                                  const std::chrono::system_clock::time_point& startTime);

// Oblicza koszt (liczbę przesiadek) trasy wyznaczonej na podstawie danego porządku przystanków
int calculate_cost(const std::vector<std::string>& order,
                   const std::vector<edge>& edges,
                   const std::string& start,
                   const std::string& end,
                   const std::chrono::system_clock::time_point& startTime);

// Główna funkcja tabu_search, wyznaczająca trasę o minimalnej liczbie przesiadek.
// Jeśli lista required_stops jest pusta, wywołuje funkcję astar_change.
std::vector<edge> tabu_search(const std::vector<edge>& edges,
                              const std::string& start,
                              const std::string& end,
                              const std::vector<std::string>& required_stops,
                              const std::chrono::system_clock::time_point& startTime,
                              int max_iterations = 100);

#endif // TABU_SEARCH_H
