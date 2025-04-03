#ifndef TABU_SEARCH_KNOX_H
#define TABU_SEARCH_KNOX_H

#include <vector>
#include <string>
#include <deque>
#include <unordered_set>
#include <chrono>
#include "../graph/edge.h"          // Deklaracja klasy edge
#include "astar.h"         // Deklaracja funkcji astar_change
#include "tabu_search.h"   // Deklaracje funkcji pomocniczych: generate_neighbors, count_transfers, construct_route, calculate_cost oraz struktury VectorHash

/**
 * @brief Funkcja realizująca algorytm Tabu Search (Knox) dla problemu komiwojażera.
 *
 * Funkcja przyjmuje:
 * - adj: listę sąsiedztwa grafu,
 * - edges: wektor krawędzi,
 * - start: przystanek początkowy,
 * - end: przystanek końcowy,
 * - required_stops: lista przystanków pośrednich (przekazywana przez wartość, aby umożliwić modyfikację kolejności),
 * - startTime: czas rozpoczęcia podróży,
 * - step_limit: maksymalna liczba iteracji zewnętrznej (kroków ogólnych),
 * - op_limit: maksymalna liczba iteracji wewnętrznej (operacji na sąsiedztwie).
 *
 * @return Para, w której pierwszy element to wyznaczona trasa (wektor edge), a drugi to koszt (np. liczba przesiadek).
 */
std::pair<std::vector<edge>, double> tabu_search_knox(
    std::unordered_map<std::string, std::vector<edge>> &adj,
    const std::vector<edge> &edges,
    const std::string &start,
    const std::string &end,
    std::vector<std::string> required_stops,
    const std::chrono::system_clock::time_point &startTime,
    int step_limit,
    int op_limit);

#endif // TABU_SEARCH_KNOX_H
