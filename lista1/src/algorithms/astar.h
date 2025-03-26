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

/**
 * @brief Stała do przeliczenia stopni na radiany.
 */
constexpr double PI = 3.14159265358979323846;

/**
 * @brief Oblicza odległość między dwoma punktami na Ziemi metodą haversine.
 *
 * Funkcja przyjmuje współrzędne geograficzne dwóch punktów (w stopniach)
 * i zwraca odległość między nimi w kilometrach.
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
 * Funkcja szacuje czas podróży na podstawie odległości (obliczonej metodą haversine)
 * przy założeniu średniej prędkości 30 km/h. Wynik zwracany jest jako czas (std::chrono::duration).
 *
 * @param current Nazwa przystanku początkowego.
 * @param target Nazwa przystanku docelowego.
 * @param edges Lista krawędzi, wykorzystywana do wyznaczenia współrzędnych przystanków.
 * @return std::chrono::system_clock::duration Szacowany czas podróży.
 */
std::chrono::system_clock::duration heuristic(const std::string &current,
                                                const std::string &target,
                                                const std::vector<edge> &edges);

/**
 * @struct AStarState
 * @brief Reprezentuje stan w algorytmie A* optymalizującym czas przejazdu.
 *
 * Struktura przechowuje aktualny przystanek, rzeczywisty czas przybycia (g),
 * dotychczasową trasę oraz szacowany koszt f = g + h, gdzie h jest wynikiem funkcji heurystycznej.
 */
struct AStarState {
    std::string stop;  ///< Aktualny przystanek
    std::chrono::system_clock::time_point time;  ///< Rzeczywisty czas przybycia (g)
    std::vector<edge> route;  ///< Dotychczasowa trasa (ciąg krawędzi)
    std::chrono::system_clock::time_point estimated; ///< Szacowany koszt f = g + h

    /**
     * @brief Operator porównania wykorzystywany przez kolejkę priorytetową.
     *
     * Porównuje stany na podstawie wartości estimated.
     *
     * @param other Inny stan do porównania.
     * @return true, jeśli bieżący stan ma większy koszt f od stanu other.
     */
    bool operator>(const AStarState &other) const;
};

/**
 * @brief Funkcja A* wyszukująca trasę o minimalnym czasie przejazdu.
 *
 * Funkcja wykorzystuje algorytm A* do wyznaczenia trasy od przystanku startowego
 * do docelowego, przy czym funkcja heurystyczna szacuje czas na podstawie odległości.
 * Koszt trasy jest obliczany jako różnica czasu (w sekundach) między przybyciem a czasem rozpoczęcia.
 *
 * @param edges Lista krawędzi (połączeń).
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para zawierająca wyznaczoną trasę oraz koszt trasy (czas w sekundach).
 */
std::pair<std::vector<edge>, double> astar_time(const std::vector<edge> &edges,
                             const std::string &start,
                             const std::string &end,
                             const std::chrono::system_clock::time_point &startTime);

/**
 * @brief Struktura stanu dla A* optymalizującego liczbę przesiadek.
 *
 * Struktura przechowuje aktualny przystanek, czas przybycia, liczbę przesiadek wykonanych do tej pory,
 * dotychczasową trasę oraz szacowany koszt f = transfers + h, gdzie h jest przyjmowane jako 0 (heurystyka trywialna).
 */
struct TransferState {
    std::string stop;  ///< Aktualny przystanek
    std::chrono::system_clock::time_point time;  ///< Czas przybycia (do sprawdzania warunków odjazdu)
    int transfers;     ///< Liczba przesiadek wykonanych do tej pory
    std::vector<edge> route;  ///< Dotychczasowa trasa (ciąg krawędzi)
    int estimated;     ///< Szacowany koszt f = transfers + h (tu h = 0)

    /**
     * @brief Operator porównania wykorzystywany przez kolejkę priorytetową.
     *
     * Porównuje stany na podstawie szacowanego kosztu (estimated).
     *
     * @param other Inny stan do porównania.
     * @return true, jeśli bieżący stan ma wyższy szacowany koszt niż stan other.
     */
    bool operator>(const TransferState &other) const;
};

/**
 * @brief Struktura przechowująca najlepszy stan dla danego przystanku.
 *
 * Używana przy wyznaczaniu trasy minimalizującej liczbę przesiadek, aby dla każdego przystanku
 * zapamiętać najlepszy (minimalny) dotychczasowy wynik.
 */
struct BestState {
    int transfers;  ///< Minimalna liczba przesiadek dotychczas uzyskana dla przystanku
    std::chrono::system_clock::time_point time; ///< Najwcześniejszy czas przybycia dla tej liczby przesiadek
};

/**
 * @brief Funkcja A* wyszukująca trasę o minimalnym czasie przejazdu z uwzględnieniem heurystyki.
 *
 * Funkcja ta realizuje algorytm A* analogiczny do klasycznego Dijkstry, lecz z dodatkową
 * funkcją heurystyczną szacującą czas podróży. Dla każdego przystanku przeliczany jest koszt
 * f = g + h, gdzie g to rzeczywisty czas przybycia, a h jest szacowany na podstawie odległości.
 *
 * @param edges Lista krawędzi.
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para zawierająca wyznaczoną trasę oraz koszt trasy (czas w sekundach).
 */
std::pair<std::vector<edge>, double> astar_time(const std::vector<edge> &edges,
                             const std::string &start,
                             const std::string &end,
                             const std::chrono::system_clock::time_point &startTime);

/**
 * @brief Funkcja A* wyszukująca trasę o minimalnej liczbie przesiadek.
 *
 * Funkcja wyszukuje trasę od przystanku startowego do docelowego, minimalizując liczbę przesiadek.
 * Podczas przeszukiwania sprawdzane jest, czy następuje zmiana linii (porównanie bieżącej linii z poprzednią)
 * – w takim przypadku licznik przesiadek jest zwiększany. Funkcja stosuje kolejkę priorytetową, a jako kryterium
 * porównania używa się stanu z mniejszym kosztem (f = transfers + h, gdzie h jest przyjmowane jako 0).
 *
 * @param edges Lista krawędzi (połączeń).
 * @param start Nazwa przystanku początkowego.
 * @param end Nazwa przystanku docelowego.
 * @param startTime Czas rozpoczęcia podróży.
 * @return std::pair<std::vector<edge>, double> Para zawierająca wyznaczoną trasę oraz koszt trasy (liczba przesiadek).
 */
std::pair<std::vector<edge>, double> astar_change(const std::vector<edge> &edges,
                               const std::string &start,
                               const std::string &end,
                               const std::chrono::system_clock::time_point &startTime);

#endif // ASTAR_H
