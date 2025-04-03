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

/**
 * @brief Funktor obliczający wartość skrótu (hash) dla wektora stringów.
 *
 * Funkcja wykorzystuje std::hash<string> oraz technikę mieszania bitów, aby
 * uzyskać unikalną wartość haszującą dla danego wektora. Umożliwia to
 * wykorzystanie std::vector<std::string> jako klucza w strukturach takich jak unordered_set.
 *
 * @param v Wektor stringów do zahaszowania.
 * @return size_t Obliczona wartość haszująca.
 */
struct VectorHash {
    size_t operator()(const std::vector<std::string>& v) const;
};

/**
 * @brief Generuje sąsiadów danego porządku przystanków.
 *
 * Funkcja tworzy wszystkie możliwe permutacje uzyskiwane przez zamianę miejscami
 * par elementów wektora. Każda taka permutacja jest potencjalnym kandydatem na
 * lepsze rozwiązanie w ramach algorytmu Tabu Search.
 *
 * @param current Wektor reprezentujący aktualny porządek przystanków.
 * @return std::vector<std::vector<std::string>> Lista wygenerowanych sąsiadów.
 */
std::vector<std::vector<std::string>> generate_neighbors(const std::vector<std::string>& current);

/**
 * @brief Oblicza liczbę przesiadek na trasie.
 *
 * Funkcja iteruje po trasie reprezentowanej przez wektor obiektów edge i liczy
 * przesiadki, przy czym zmiana linii (porównywana za pomocą metody getLine()) jest
 * uznawana za przesiadkę.
 *
 * @param route Wektor krawędzi reprezentujący trasę.
 * @return int Liczba przesiadek na trasie.
 */
int count_transfers(const std::vector<edge>& route);

/**
 * @brief Buduje pełną trasę łącząc segmenty wyznaczone funkcją astar_change.
 *
 * Funkcja przetwarza kolejno podany porządek przystanków, wyznaczając segmenty trasy
 * między przystankiem początkowym a kolejnymi wymaganymi przystankami, aż do przystanku docelowego.
 * Jeśli dla któregoś segmentu nie uda się znaleźć połączenia, zwracany jest pusty wektor.
 *
 * @param order Kolejność odwiedzanych przystanków.
 * @param edges Wektor krawędzi wykorzystywany przez funkcję astar_change.
 * @param start Przystanek początkowy.
 * @param end Przystanek docelowy.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::vector<edge> Zbudowana trasa.
 */
std::vector<edge> construct_route(const std::vector<std::string> &order,
                             std::unordered_map<std::string, std::vector<edge>> &adj,
                             const std::vector<edge> &edges,
                             const std::string &start,
                             const std::string &end,
                             const std::chrono::system_clock::time_point &startTime);

/**
 * @brief Oblicza koszt trasy na podstawie liczby przesiadek.
 *
 * Funkcja buduje trasę przy pomocy funkcji construct_route, a następnie
 * wyznacza koszt (liczbę przesiadek) używając count_transfers. Jeśli trasy
 * nie uda się wyznaczyć, zwraca INT_MAX.
 *
 * @param order Kolejność przystanków do odwiedzenia.
 * @param edges Wektor krawędzi.
 * @param start Przystanek początkowy.
 * @param end Przystanek docelowy.
 * @param startTime Czas rozpoczęcia podróży.
 * @return double Koszt trasy (liczba przesiadek) lub INT_MAX, jeśli trasa nie istnieje.
 */
double calculate_cost(const std::vector<std::string> &order,
                      std::unordered_map<std::string, std::vector<edge>> &adj,
                      const std::vector<edge> &edges,
                      const std::string &start,
                      const std::string &end,
                      const std::chrono::system_clock::time_point &startTime);

/**
 * @brief Główna funkcja algorytmu Tabu Search.
 *
 * Funkcja wyszukuje trasę o minimalnej liczbie przesiadek przy zadanym porządku przystanków.
 * Jeśli nie podano przystanków pośrednich, wykorzystuje bezpośrednią metodę astar_change.
 * W kolejnych iteracjach generowane są sąsiednie rozwiązania (permutacje porządku przystanków),
 * a następnie wybierany jest najlepszy kandydat, z uwzględnieniem listy tabu (zapobiegającej cyklom)
 * oraz warunku aspiracji (akceptującego lepsze globalnie rozwiązania).
 *
 * @param adj Wygenerowany graf
 * @param edges Wektor krawędzi.
 * @param start Przystanek początkowy.
 * @param end Przystanek docelowy.
 * @param required_stops Lista przystanków pośrednich do odwiedzenia.
 * @param startTime Czas rozpoczęcia podróży.
 * @param max_iterations Maksymalna liczba iteracji algorytmu.
 * @return std::pair<std::vector<edge>, double> Parę zawierającą najlepszą znalezioną trasę oraz jej koszt.
 */
std::pair<std::vector<edge>, double> tabu_search(std::unordered_map<std::string, std::vector<edge>> &adj,
                                                 const std::vector<edge>& edges,
                                                 const std::string& start,
                                                 const std::string& end,
                                                 const std::vector<std::string>& required_stops,
                                                 const std::chrono::system_clock::time_point& startTime,
                                                 int max_iterations = 100);

std::pair<std::vector<edge>, double> tabu_search_change(
    std::unordered_map<std::string, std::vector<edge>> &adj,
    const std::vector<edge> &edges,
    const std::string &start,
    const std::string &end,
    const std::vector<std::string> &required_stops,
    const std::chrono::system_clock::time_point &startTime,
    int max_iterations);

#endif // TABU_SEARCH_H
