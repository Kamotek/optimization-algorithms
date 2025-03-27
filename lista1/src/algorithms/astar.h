#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <sstream>
#include "../graph/edge.h"

/**
 * @brief Stała do przeliczenia stopni na radiany.
 */
constexpr double PI = 3.14159265358979323846;

/**
 * @brief Oblicza odległość między dwoma punktami na Ziemi metodą haversine.
 *
 * @param lat1 Szerokość geograficzna pierwszego punktu.
 * @param lon1 Długość geograficzna pierwszego punktu.
 * @param lat2 Szerokość geograficzna drugiego punktu.
 * @param lon2 Długość geograficzna drugiego punktu.
 * @return double Odległość między punktami w kilometrach.
 */
double haversine(double lat1, double lon1, double lat2, double lon2);

/**
 * @brief Funkcja heurystyczna szacująca czas przejazdu pomiędzy przystankami.
 *
 * @param current Nazwa przystanku początkowego.
 * @param target Nazwa przystanku docelowego.
 * @param edges Lista krawędzi (używana do wyznaczenia współrzędnych przystanków).
 * @return std::chrono::system_clock::duration Szacowany czas podróży.
 */
std::chrono::system_clock::duration heuristic(const std::string &current,
                                                const std::string &target,
                                                const std::vector<edge> &edges);

/**
 * @struct AStarState
 * @brief Reprezentuje stan w algorytmie A* optymalizującym czas przejazdu.
 */
struct AStarState {
    std::string stop;  ///< Aktualny przystanek
    std::chrono::system_clock::time_point time;  ///< Rzeczywisty czas przybycia (g)
    std::vector<edge> route;  ///< Dotychczasowa trasa (ciąg krawędzi)
    std::chrono::system_clock::time_point estimated; ///< Szacowany koszt f = g + h

    bool operator>(const AStarState &other) const;
};

/**
 * @brief Funkcja A* wyszukująca trasę o minimalnym czasie przejazdu z uwzględnieniem heurystyki.
 *
 * @param adj Wygenerowany graf (lista sąsiedztwa).
 * @param edges Lista krawędzi (do celów obliczeń heurystycznych).
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para zawierająca wyznaczoną trasę oraz koszt trasy (czas w sekundach).
 */
std::pair<std::vector<edge>, double> astar_time(
    const std::unordered_map<std::string, std::vector<edge>>& adj,
    const std::vector<edge>& edges,
    const std::string &start,
    const std::string &end,
    const std::chrono::system_clock::time_point &startTime);

/**
 * @brief Struktura stanu dla A* optymalizującego liczbę przesiadek.
 */
struct TransferState {
    std::string stop;  ///< Aktualny przystanek
    std::chrono::system_clock::time_point time;  ///< Czas przybycia
    int transfers;     ///< Liczba przesiadek wykonanych do tej pory
    std::vector<edge> route;  ///< Dotychczasowa trasa (ciąg krawędzi)
    int estimated;     ///< Szacowany koszt f = transfers (tu h = 0)

    bool operator>(const TransferState &other) const;
};

/**
 * @brief Struktura przechowująca najlepszy stan dla danego przystanku.
 */
struct BestState {
    int transfers;  ///< Minimalna liczba przesiadek dotychczas uzyskana
    std::chrono::system_clock::time_point time; ///< Najwcześniejszy czas przybycia dla tej liczby przesiadek
};

/**
 * @brief Funkcja A* wyszukująca trasę o minimalnej liczbie przesiadek.
 *
 * @param adj Wygenerowany graf (lista sąsiedztwa).
 * @param edges Lista krawędzi.
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para zawierająca wyznaczoną trasę oraz koszt trasy (liczba przesiadek).
 */
std::pair<std::vector<edge>, double> astar_change(
    std::unordered_map<std::string, std::vector<edge>>& adj,
    const std::vector<edge>& edges,
    const std::string &start,
    const std::string &end,
    const std::chrono::system_clock::time_point &startTime);

#endif // ASTAR_H
