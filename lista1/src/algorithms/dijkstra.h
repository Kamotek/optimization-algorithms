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

/**
 * @struct State
 * @brief Reprezentuje stan w algorytmie Dijkstry, przyjmowany jako element kolejki priorytetowej.
 *
 * Struktura ta przechowuje informacje o aktualnym przystanku, czasie przybycia oraz ścieżce (ciągu krawędzi)
 * prowadzącej do tego przystanku.
 */
struct State {
    std::string stop;  ///< Nazwa aktualnego przystanku
    std::chrono::system_clock::time_point time;  ///< Czas przybycia na ten przystanek
    std::vector<edge> route;  ///< Ścieżka (ciąg krawędzi) prowadząca do przystanku

    /**
     * @brief Operator porównania wykorzystywany przez kolejkę priorytetową.
     *
     * Porównuje stany na podstawie czasu przybycia (mniejsze czasy mają wyższy priorytet).
     *
     * @param other Inny stan do porównania.
     * @return true, jeśli czas bieżącego stanu jest większy od czasu stanu 'other'.
     */
    bool operator>(const State& other) const;
};

/**
 * @struct StateChange
 * @brief Reprezentuje stan w algorytmie Dijkstry uwzględniającym liczbę przesiadek.
 *
 * Oprócz danych z klasy State, struktura ta przechowuje liczbę przesiadek oraz
 * nazwę ostatniej użytej linii komunikacyjnej. Umożliwia to wyszukiwanie tras o minimalnej liczbie przesiadek.
 */
struct StateChange {
    std::string stop;  ///< Nazwa aktualnego przystanku
    std::chrono::system_clock::time_point time;  ///< Czas przybycia na przystanek
    int transfers;  ///< Liczba wykonanych przesiadek
    std::string current_line; ///< Nazwa ostatniej użytej linii (początkowo pusta)
    std::vector<edge> route;  ///< Ścieżka prowadząca do tego stanu

    /**
     * @brief Operator porównania wykorzystywany przez kolejkę priorytetową.
     *
     * Porównuje stany na podstawie liczby przesiadek (mniejsza liczba ma wyższy priorytet).
     *
     * @param other Inny stan do porównania.
     * @return true, jeśli liczba przesiadek w bieżącym stanie jest większa niż w stanie 'other'.
     */
    bool operator>(const StateChange& other) const;
};

/**
 * @brief Funkcja Dijkstry wyszukująca najkrótszą trasę według kryterium czasu.
 *
 * Funkcja przeszukuje graf (przedstawiony jako lista krawędzi) od przystanku startowego do docelowego,
 * wybierając trasy o najkrótszym czasie przejazdu. Funkcja wykorzystuje kolejkę priorytetową,
 * a funkcję kosztu definiuje jako różnicę czasu (w sekundach) między momentem przybycia a czasem rozpoczęcia.
 *
 * @param adj Wygenerowany graf
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para, gdzie pierwszy element to wyznaczona trasa,
 *         a drugi element to koszt trasy (różnica czasu w sekundach). W przypadku braku trasy zwraca parę {pusta trasa, -1.0}.
 */
std::pair<std::vector<edge>, double> dijkstra_time(std::unordered_map<std::string, std::vector<edge>>& adj,
                                const std::string& start,
                                const std::string& end,
                                const std::chrono::system_clock::time_point& startTime
                                );

/**
 * @brief Funkcja Dijkstry wyszukująca trasę z uwzględnieniem liczby przesiadek.
 *
 * Funkcja wyszukuje trasę od przystanku startowego do docelowego, minimalizując liczbę przesiadek.
 * W algorytmie brane są pod uwagę zmiany linii – każda zmiana linii zwiększa licznik przesiadek.
 *
 * @param edges Lista krawędzi reprezentujących połączenia.
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para, gdzie pierwszy element to wyznaczona trasa,
 *         a drugi element to koszt trasy (liczba przesiadek). Jeśli trasa nie zostanie znaleziona,
 *         zwracana jest para {pusta trasa, -1.0}.
 */
std::pair<std::vector<edge>, double> dijkstra_change(std::unordered_map<std::string, std::vector<edge>>& adj,
                                const std::string& start,
                                const std::string& end,
                                const std::chrono::system_clock::time_point& startTime);

#endif // DIJKSTRA_H
