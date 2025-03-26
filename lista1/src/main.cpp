// main.cpp
#include <iostream>
#include <vector>
#include "io_handling/csv_reader.h"
#include "graph/graph_generator.h"
#include "graph/graph.h"
#include "ui/user_cli.h"
#include "graph/edge.h"
#include <fcntl.h>
#include <io.h>
#include <windows.h>

// Funkcja pomocnicza do wypisania trasy
void printRoute(const std::vector<edge>& route) {
    if (route.empty()) {
        std::cout << "Nie znaleziono trasy." << std::endl;
        return;
    }
    for (const auto& e : route) {
        std::cout << e.to_str() << std::endl;
    }
}

int main() {
    SetConsoleCP(CP_UTF8); // Ustaw konsolę na UTF-8
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());


    // Wczytanie danych z pliku CSV
    std::vector<std::string> data = readCSVFile("../data/connection_graph.csv");
    if (data.empty()) {
        std::cerr << "Błąd wczytywania danych z pliku CSV." << std::endl;
        return 1;
    }
    data.erase(data.begin()); // Usuwamy nagłówek
    graph_generator generator(data);
    std::vector<edge> edges = generator.get_graphs();

    // Budujemy graf (jeśli jest potrzebny w dalszej logice)
    // Graph graph;
    // graph.buildGraph(edges);

    // Utworzenie obiektu CLI, który pobiera wszystkie dane i wybiera algorytm
    user_cli cli;
    // Wykonanie algorytmu na podstawie danych z CLI
    std::vector<edge> bestRoute = cli.execute(edges);

    // Wypisanie wyników
    std::cout << "\n==== Wynik trasy ====" << std::endl;
    printRoute(bestRoute);

    return 0;
}
