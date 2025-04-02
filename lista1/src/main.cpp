// main.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>   // Dla std::setprecision
#include "io_handling/csv_reader.h"
#include "graph/graph_generator.h"
#include "graph/graph.h"
#include "ui/user_cli.h"
#include "graph/edge.h"
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#endif

// Funkcja pomocnicza do wypisywania dzielnika sekcji
void printDivider(const std::string &title) {
    std::cout << "\n========== " << title << " ==========\n";
}

// Funkcja pomocnicza do wypisania trasy
void printRoute(const std::vector<edge>& route) {
    if (route.empty()) {
        std::cout << "Nie znaleziono trasy." << std::endl;
        return;
    }
    for (const auto& e : route) {
        std::cout << "-> " << e.to_str() << std::endl;
    }
}

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8); // Ustawienie konsoli na UTF-8
#endif
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    std::vector<std::string> data = readCSVFile("../data/connection_graph.csv");
    if (data.empty()) {
        std::cerr << "Błąd wczytywania danych z pliku CSV." << std::endl;
        return 1;
    }
    data.erase(data.begin()); // Usuwamy nagłówek

    graph_generator generator(data);
    std::vector<edge> edges = generator.get_graphs();



    user_cli cli;

    std::pair<std::vector<edge>, double> result = cli.execute(edges);


    std::vector<edge> bestRoute = result.first;
    double bestCost = result.second;

    printDivider("Wynik trasy");
    printRoute(bestRoute);

    printDivider("Koszt");
    std::cout << "Koszt trasy: " << bestCost << std::endl;


    return 0;
}
