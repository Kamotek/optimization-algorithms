//
// Created by kamil on 23.03.2025.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <chrono>
#include "../graph/edge.h"  // Pełna definicja klasy edge

// Struktura reprezentująca stan w algorytmie Dijkstry.
struct State {
    std::string stop;  // aktualny przystanek
    std::chrono::system_clock::time_point time;  // czas przybycia na ten przystanek
    std::vector<edge> route;  // ścieżka (ciąg krawędzi) prowadząca do tego przystanku

    // Operator porównania – deklaracja, implementacja w pliku .cpp
    bool operator>(const State& other) const;
};

// Definicja stanu dla algorytmu, który uwzględnia przesiadki
struct StateChange {
    std::string stop;
    std::chrono::system_clock::time_point time;
    int transfers;
    std::string current_line; // Ostatnia używana linia (początkowo pusta)
    std::vector<edge> route;

    // Porównujemy stany na podstawie liczby przesiadek.
    bool operator>(const StateChange& other) const;
};

// Deklaracja funkcji Dijkstry wyszukującej trasę wg kryterium czasowego.
// Parametry:
// - edges: lista wszystkich krawędzi (linii)
// - start: nazwa przystanku początkowego
// - end: nazwa przystanku docelowego
// - startTime: czas rozpoczęcia podróży (od którego należy szukać połączeń)
std::vector<edge> dijkstra_time(const std::vector<edge>& edges,
                                const std::string& start,
                                const std::string& end,
                                const std::chrono::system_clock::time_point& startTime);


std::vector<edge> dijkstra_change(const std::vector<edge>& edges,
                                const std::string& start,
                                const std::string& end,
                                const std::chrono::system_clock::time_point& startTime);


#endif // DIJKSTRA_H
