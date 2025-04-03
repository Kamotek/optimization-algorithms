#include "tabu_search_knox.h"
#include <algorithm>
#include <random>
#include <climits>

// Funkcja realizująca algorytm Knoxa (Tabu Search) dla problemu komiwojażera
std::pair<std::vector<edge>, double> tabu_search_knox(
    std::unordered_map<std::string, std::vector<edge>> &adj,
    const std::vector<edge> &edges,
    const std::string &start,
    const std::string &end,
    std::vector<std::string> required_stops,
    const std::chrono::system_clock::time_point &startTime,
    int step_limit,
    int op_limit)
{
    // Losowe przetasowanie początkowego porządku przystanków
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(required_stops.begin(), required_stops.end(), std::default_random_engine(seed));

    // Inicjalizacja początkowego rozwiązania s oraz najlepszego rozwiązania s*
    std::vector<std::string> current_order = required_stops;
    std::vector<std::string> best_order = current_order;
    // Przekazujemy adj jako drugi parametr do funkcji construct_route
    std::vector<edge> best_route = construct_route(best_order, adj, edges, start, end, startTime);
    double best_cost = best_route.empty() ? INT_MAX : count_transfers(best_route);

    // Inicjalizacja listy tabu (FIFO) i zbioru do szybkiego sprawdzania
    VectorHash hash_fn;
    std::deque<size_t> tabu_queue;
    std::unordered_set<size_t> tabu_set;
    size_t tabu_max_size = 2 * required_stops.size();

    int k = 0;
    // Pętla zewnętrzna – kroki algorytmu (STEP_LIMIT)
    while (k < step_limit) {
        int i = 0;
        // Pętla wewnętrzna – operacje na sąsiedztwie (OP_LIMIT)
        while (i < op_limit) {
            auto neighbors = generate_neighbors(current_order);
            double current_best_cost = INT_MAX;
            std::vector<std::string> current_best_order;
            bool found_aspiration = false;

            // Przegląd wszystkich sąsiadów
            for (const auto &neighbor : neighbors) {
                size_t neighbor_hash = hash_fn(neighbor);
                // Obliczamy koszt trasy dla sąsiada – przekazujemy adj jako drugi parametr
                int cost = calculate_cost(neighbor, adj, edges, start, end, startTime);

                // Warunek aspiracji: jeśli sąsiad poprawia globalny wynik, wybieramy go natychmiast
                if (cost < best_cost) {
                    current_best_cost = cost;
                    current_best_order = neighbor;
                    found_aspiration = true;
                    break;
                }

                // Pomijamy rozwiązania znajdujące się na liście tabu
                if (tabu_set.find(neighbor_hash) != tabu_set.end())
                    continue;

                if (cost < current_best_cost) {
                    current_best_cost = cost;
                    current_best_order = neighbor;
                }
            }

            // Jeśli żaden sąsiad nie został znaleziony, kończymy operacje wewnętrzne
            if (current_best_cost == INT_MAX)
                break;

            // Aktualizacja bieżącego rozwiązania: jeśli s' poprawia s, przyjmujemy s'
            int current_cost = calculate_cost(current_order, adj, edges, start, end, startTime);
            if (current_best_cost < current_cost) {
                current_order = current_best_order;
                i++;  // Zwiększamy licznik operacji wewnętrznych
            }
            else {
                // Jeśli żaden ruch nie poprawia bieżącego rozwiązania, przerywamy pętlę wewnętrzną
                break;
            }

            // Aktualizacja listy tabu – dodajemy bieżący porządek, usuwamy najstarszy, gdy lista przekroczy ustalony rozmiar
            size_t current_hash = hash_fn(current_order);
            tabu_queue.push_back(current_hash);
            tabu_set.insert(current_hash);
            if (tabu_queue.size() > tabu_max_size) {
                size_t old_hash = tabu_queue.front();
                tabu_queue.pop_front();
                tabu_set.erase(old_hash);
            }
        } // koniec pętli wewnętrznej

        // Aktualizacja globalnego najlepszego rozwiązania, jeśli bieżące jest lepsze
        int current_cost = calculate_cost(current_order, adj, edges, start, end, startTime);
        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_order = current_order;
            best_route = construct_route(best_order, adj, edges, start, end, startTime);
        }
        k++; // kolejny krok zewnętrzny
    }

    return {best_route, best_cost};
}
