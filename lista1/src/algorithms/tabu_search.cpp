/**
 * @file tabu_search.cpp
 * @brief Implementacja algorytmu Tabu Search wraz z funkcjami pomocniczymi.
 *
 * Ten plik zawiera implementację funkcji wspomagających dla algorytmu Tabu Search,
 * m.in. funkcje generujące sąsiadów, liczące przesiadki oraz budujące trasę, a także
 * główną funkcję tabu_search.
 */

#include "tabu_search.h"
#include "astar.h"  // Zawiera deklarację zmodyfikowanej wersji funkcji astar_change przyjmującej listę sąsiedztwa
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

//-----------------------------------------------------------------------------
// Funkcja hashująca wektor stringów (używana przy liście tabu)
size_t VectorHash::operator()(const vector<string>& v) const {
    hash<string> hasher;
    size_t seed = 0;
    for (const auto& s : v) {
        seed ^= hasher(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

//-----------------------------------------------------------------------------
// Generowanie sąsiadów (wszystkich możliwych permutacji przez zamianę dwóch elementów)
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

//-----------------------------------------------------------------------------
// Funkcja licząca liczbę przesiadek w danej trasie
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

//-----------------------------------------------------------------------------
// Funkcja budująca pełną trasę na podstawie zadanego porządku przystanków
vector<edge> construct_route(const vector<string> &order,
                             std::unordered_map<std::string, std::vector<edge>> &adj,
                             const vector<edge> &edges,
                             const string &start,
                             const string &end,
                             const system_clock::time_point &startTime) {
    vector<edge> full_route;
    string current_stop = start;
    auto current_time = startTime;

    // Iteracja po kolejnych przystankach pośrednich
    for (const auto& next_stop : order) {
        // Używamy zmodyfikowanej funkcji astar_change, która przyjmuje listę sąsiedztwa
        auto segment = astar_change(adj, edges, current_stop, next_stop, current_time).first;
        if (segment.empty()) return {}; // Brak połączenia dla danego segmentu
        full_route.insert(full_route.end(), segment.begin(), segment.end());
        current_stop = next_stop;
        current_time = segment.back().getArrivalTime();
    }

    // Łączymy ostatni segment od ostatniego przystanku pośredniego do przystanku docelowego
    auto final_segment = astar_change(adj, edges, current_stop, end, current_time).first;
    if (final_segment.empty()) return {};
    full_route.insert(full_route.end(), final_segment.begin(), final_segment.end());

    return full_route;
}

//-----------------------------------------------------------------------------
// Funkcja obliczająca koszt trasy (liczbę przesiadek) dla zadanego porządku przystanków
double calculate_cost(const vector<string> &order,
                      std::unordered_map<std::string, std::vector<edge>> &adj,
                      const vector<edge> &edges,
                      const string &start,
                      const string &end,
                      const system_clock::time_point &startTime) {
    auto route = construct_route(order, adj, edges, start, end, startTime);
    return route.empty() ? INT_MAX : count_transfers(route);
}

//-----------------------------------------------------------------------------
// Główna funkcja Tabu Search wykorzystująca zbudowaną listę sąsiedztwa
pair<vector<edge>, double> tabu_search(std::unordered_map<std::string, std::vector<edge>> &adj,
                                       const vector<edge> &edges,
                                       const string &start,
                                       const string &end,
                                       const vector<string> &required_stops,
                                       const system_clock::time_point &startTime,
                                       int max_iterations) {
    // Jeśli nie podano przystanków pośrednich, szukamy bezpośredniej trasy (używając astar_change)
    if (required_stops.empty()) {
        auto route = astar_change(adj, edges, start, end, startTime).first;
        double cost = route.empty() ? INT_MAX : count_transfers(route);
        return {route, cost};
    }

    // Inicjalizacja bieżącego porządku przystanków pośrednich oraz globalnie najlepszego rozwiązania
    vector<string> current_order = required_stops;
    vector<string> best_order = current_order;
    vector<edge> best_route = construct_route(best_order, adj, edges, start, end, startTime);
    double best_cost = best_route.empty() ? INT_MAX : count_transfers(best_route);

    VectorHash hash_fn;
    // Lista tabu realizowana jako kolejka FIFO
    deque<size_t> tabu_queue;
    unordered_set<size_t> tabu_set;
    // Rozmiar listy tabu ustawiony na 2 * liczba przystanków pośrednich
    size_t tabu_max_size = 2 * required_stops.size();

    for (int iter = 0; iter < max_iterations; ++iter) {
        auto neighbors = generate_neighbors(current_order);
        double current_best_cost = INT_MAX;
        vector<string> current_best_order;

        // Przeglądanie wszystkich sąsiadów
        for (const auto& neighbor : neighbors) {
            size_t neighbor_hash = hash_fn(neighbor);
            int cost = calculate_cost(neighbor, adj, edges, start, end, startTime);

            // Warunek aspiracji: jeśli sąsiad poprawia globalny wynik, wybieramy go natychmiast
            if (cost < best_cost) {
                current_best_cost = cost;
                current_best_order = neighbor;
                break;
            }

            // Pomijamy rozwiązania znajdujące się na liście tabu
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
            best_route = construct_route(best_order, adj, edges, start, end, startTime);
        }

        // Aktualizacja listy tabu – dodajemy bieżący porządek i usuwamy najstarszy, gdy przekroczymy limit
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

/**
 * @brief Funkcja tabu_search_change wyszukuje trasę minimalizującą liczbę przesiadek.
 *
 * Przyjmuje:
 * - adj: listę sąsiedztwa grafu,
 * - edges: listę wszystkich krawędzi,
 * - start: przystanek początkowy,
 * - end: przystanek końcowy,
 * - required_stops: lista przystanków do odwiedzenia (pośrednich),
 * - startTime: czas rozpoczęcia podróży,
 * - max_iterations: maksymalna liczba iteracji algorytmu.
 *
 * Jeśli nie podano przystanków pośrednich, funkcja próbuje znaleźć trasę bezpośrednią
 * korzystając z funkcji astar_change.
 *
 * Funkcja zwraca parę, gdzie pierwszy element to znaleziona trasa (wektor edge), a drugi element
 * to koszt (liczba przesiadek).
 */
std::pair<std::vector<edge>, double> tabu_search_change(
    std::unordered_map<std::string, std::vector<edge>> &adj,
    const std::vector<edge> &edges,
    const std::string &start,
    const std::string &end,
    const std::vector<std::string> &required_stops,
    const std::chrono::system_clock::time_point &startTime,
    int max_iterations)
{
    // Jeśli nie podano przystanków pośrednich, szukamy bezpośredniej trasy za pomocą astar_change
    if (required_stops.empty()) {
        auto route = astar_change(adj, edges, start, end, startTime).first;
        double cost = route.empty() ? INT_MAX : count_transfers(route);
        return {route, cost};
    }

    // Inicjalizacja bieżącego porządku przystanków pośrednich oraz globalnie najlepszego rozwiązania
    std::vector<std::string> current_order = required_stops;
    std::vector<std::string> best_order = current_order;
    std::vector<edge> best_route = construct_route(best_order, adj, edges, start, end, startTime);
    double best_cost = best_route.empty() ? INT_MAX : count_transfers(best_route);

    // Funkcja hashująca porządki, używana do implementacji listy tabu
    VectorHash hash_fn;
    // Lista tabu realizowana jako kolejka FIFO oraz zbiór ułatwiający szybkie sprawdzanie
    std::deque<size_t> tabu_queue;
    std::unordered_set<size_t> tabu_set;
    // Rozmiar listy tabu ustawiony na 2 * liczba przystanków pośrednich (można modyfikować dla poprawy wyników)
    size_t tabu_max_size = 2 * required_stops.size();

    for (int iter = 0; iter < max_iterations; ++iter) {
        auto neighbors = generate_neighbors(current_order);
        double current_best_cost = INT_MAX;
        std::vector<std::string> current_best_order;

        // Przegląd sąsiadów (permutacji kolejności przystanków pośrednich)
        for (const auto &neighbor : neighbors) {
            size_t neighbor_hash = hash_fn(neighbor);
            int cost = count_transfers( construct_route(neighbor, adj, edges, start, end, startTime) );

            // Warunek aspiracji: jeśli sąsiad poprawia globalny wynik, wybieramy go natychmiast
            if (cost < best_cost) {
                current_best_cost = cost;
                current_best_order = neighbor;
                break;
            }

            // Pomijamy rozwiązania znajdujące się na liście tabu
            if (tabu_set.find(neighbor_hash) != tabu_set.end()) continue;

            if (cost < current_best_cost) {
                current_best_cost = cost;
                current_best_order = neighbor;
            }
        }

        // Jeśli nie znaleziono żadnego sąsiada, który poprawiałby rozwiązanie, kończymy iteracje
        if (current_best_cost == INT_MAX)
            break;

        // Aktualizacja globalnie najlepszego rozwiązania
        if (current_best_cost < best_cost) {
            best_cost = current_best_cost;
            best_order = current_best_order;
            best_route = construct_route(best_order, adj, edges, start, end, startTime);
        }

        // Aktualizacja listy tabu – dodajemy bieżący porządek, usuwamy najstarszy, gdy przekroczymy limit
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
