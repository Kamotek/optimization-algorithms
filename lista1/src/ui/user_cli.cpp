// user_cli.cpp
#include "user_cli.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>

// Zakładamy, że funkcje algorytmów są zadeklarowane w odpowiednich nagłówkach
#include "../algorithms/dijkstra.h"
#include "../algorithms/astar.h"
#include "../algorithms/tabu_search.h"


user_cli::user_cli() {
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    gatherBasicData();
    gatherAlgorithmChoice();
}

void user_cli::gatherBasicData() {
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::setlocale(LC_ALL, "pl_PL.UTF-8");

    std::cout << "Czy skorzystać z domyślnych ustawień? [T/N]: ";
    std::string useDefaultStr;
    std::getline(std::cin, useDefaultStr);
    char useDefault = useDefaultStr.empty() ? 'N' : useDefaultStr[0];

    if (useDefault == 'T' || useDefault == 't') {
        start_stop = "BISKUPIN";
        end_stop = "KOZANÓW";
        optimization_criteria = 't'; // domyślnie czasowe
        start_time = std::chrono::system_clock::now();
    } else {
        std::cout << "Podaj przystanek początkowy: ";
        std::getline(std::cin, start_stop);

        std::cout << "Podaj przystanek końcowy: ";
        std::getline(std::cin, end_stop);

        std::cout << "Podaj czas (HH:MM:SS): ";
        std::string time_str;
        std::getline(std::cin, time_str);

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        tm local_tm = *std::localtime(&now_time); // Pobierz aktualną datę

        // Ustaw tylko czas z zachowaniem aktualnej daty
        if (sscanf(time_str.c_str(), "%d:%d:%d", &local_tm.tm_hour, &local_tm.tm_min, &local_tm.tm_sec) == 3) {
            start_time = std::chrono::system_clock::from_time_t(std::mktime(&local_tm))+std::chrono::hours(1);

        } else {
            std::cerr << "Niepoprawny format czasu. Używam aktualnego czasu." << std::endl;
            start_time = now;
        }

        std::cout << "Wybierz kryterium (t - najkrótszy dojazd, p - najmniej przesiadek): ";
        std::string crit;
        std::getline(std::cin, crit);
        if (!crit.empty()) {
            optimization_criteria = crit[0];
        }
    }
}

void user_cli::gatherAlgorithmChoice() {
    std::cout << "\nWybrano trasę: " << start_stop << " -> " << end_stop << std::endl;
    std::cout << "Kryterium optymalizacji: "
              << (optimization_criteria == 't' ? "czasowe" : "przesiadkowe")
              << std::endl;

    std::cout << "\nWybierz algorytm do wykonania:" << std::endl;
    if (optimization_criteria == 't') {
        std::cout << "1. Dijkstra (czasowy)" << std::endl;
        std::cout << "2. A* (czasowy)" << std::endl;
    } else if (optimization_criteria == 'p') {
        std::cout << "1. Dijkstra (przesiadkowy)" << std::endl;
        std::cout << "2. A* (przesiadkowy)" << std::endl;
    }
    std::cout << "3. Tabu Search (z możliwością wykluczenia wybranych przystanków)" << std::endl;
    std::cout << "Wybierz opcję (1, 2, lub 3): ";
    std::string algo_str;
    std::getline(std::cin, algo_str);
    if (!algo_str.empty())
        algorithm_choice = std::stoi(algo_str);

    if (algorithm_choice == 3) {
        std::cout << "Podaj przystanki do wykluczenia (oddzielone spacjami): ";
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string stop;
        while (iss >> stop) {
            excluded_stops.push_back(stop);
        }
    }
}


bool validateStops(const std::vector<edge>& edges, const std::string& start, const std::string& end) {
    std::unordered_set<std::string> stops;
    for (const auto& e : edges) {
        stops.insert(e.getStartStop());
        stops.insert(e.getEndStop());
    }
    return stops.count(start) && stops.count(end);
}

// Modyfikacja funkcji execute
std::vector<edge> user_cli::execute(const std::vector<edge>& edges) {
    if (!validateStops(edges, start_stop, end_stop)) {
        std::cerr << "Błąd: Nieprawidłowe nazwy przystanków." << std::endl;
        std::cerr << start_stop << ", " << end_stop << std::endl;
        return {};
    }
    // W zależności od wyboru użytkownika wywołujemy odpowiedni algorytm.
    std::vector<edge> route;
    if (algorithm_choice == 1) {
        if (optimization_criteria == 't') {
            route = dijkstra_time(edges, start_stop, end_stop, start_time);
        } else {
            route = dijkstra_change(edges, start_stop, end_stop, start_time);
        }
    } else if (algorithm_choice == 2) {
        if (optimization_criteria == 't') {
            route = astar_time(edges, start_stop, end_stop, start_time);
        } else {
            route = astar_change(edges, start_stop, end_stop, start_time);
        }
    } else if (algorithm_choice == 3) {
        route = tabu_search(edges, start_stop, end_stop, excluded_stops, start_time);
    } else {
        std::cerr << "Niepoprawny wybór algorytmu." << std::endl;
    }
    return route;
}

std::string user_cli::get_start_stop() const { return start_stop; }
std::string user_cli::get_end_stop() const { return end_stop; }
char user_cli::get_optimization_criteria() const { return optimization_criteria; }
std::chrono::system_clock::time_point user_cli::get_start_time() const { return start_time; }
int user_cli::get_algorithm_choice() const { return algorithm_choice; }
std::vector<std::string> user_cli::get_excluded_stops() const { return excluded_stops; }
