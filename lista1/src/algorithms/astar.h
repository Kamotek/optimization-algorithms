//
// Created by kamil on 23.03.2025.
//

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





// Uwzględniamy ścieżkę do pliku edge.h (dostosuj ją, jeśli jest inna)
#include "../graph/edge.h"

// Stała do przeliczenia stopni na radiany
constexpr double PI = 3.14159265358979323846;

// Funkcja obliczająca odległość według wzoru haversine (wynik w kilometrach)
double haversine(double lat1, double lon1, double lat2, double lon2);

// Funkcja heurystyczna – szacuje czas przejazdu pomiędzy przystankami na podstawie odległości.
// Zakładamy średnią prędkość 30 km/h.
std::chrono::system_clock::duration heuristic(const std::string &current,
                                                const std::string &target,
                                                const std::vector<edge> &edges);

// Struktura stanu dla A*, zawiera bieżący przystanek, czas (g - koszt dojścia),
// aktualną trasę oraz szacowany koszt f = g + h.
struct AStarState {
    std::string stop;
    std::chrono::system_clock::time_point time;  // rzeczywisty czas przybycia (g)
    std::vector<edge> route;
    std::chrono::system_clock::time_point estimated; // f = time + heurystyka

    bool operator>(const AStarState &other) const;
};

// Funkcja astar_time, działająca analogicznie do dijkstra_time, lecz wykorzystująca algorytm A*.
// Parametry:
// - edges: lista krawędzi
// - start: nazwa przystanku początkowego
// - end: nazwa przystanku docelowego
// - startTime: czas rozpoczęcia podróży
std::vector<edge> astar_time(const std::vector<edge> &edges,
                             const std::string &start,
                             const std::string &end,
                             const std::chrono::system_clock::time_point &startTime);



std::vector<edge> astar_change(const std::vector<edge> &edges,
                               const std::string &start,
                               const std::string &end,
                               const std::chrono::system_clock::time_point &startTime);

// Struktura stanu dla A* optymalizującego liczbę przesiadek
struct TransferState {
    std::string stop;
    std::chrono::system_clock::time_point time;  // czas przybycia (do sprawdzania warunków odjazdu)
    int transfers;                                 // liczba przesiadek wykonanych do tej pory
    std::vector<edge> route;                       // dotychczasowa trasa
    int estimated;                                 // f = transfers + h; przyjmujemy h = 0 (heurystyka trywialna)

    bool operator>(const TransferState &other) const;
};

struct BestState {
    int transfers;
    std::chrono::system_clock::time_point time;
};



#endif // ASTAR_H



