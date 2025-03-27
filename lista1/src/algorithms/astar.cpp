//
// Created by kamil on 23.03.2025.
//

#include "astar.h"

#include <unordered_set>


// Przyjmujemy, że struktura edge jest zdefiniowana podobnie jak w poprzednich fragmentach
// i posiada metody:
//  - getStartStop(), getEndStop() zwracające std::string,
//  - getDepartureTime(), getArrivalTime() zwracające std::chrono::system_clock::time_point,
//  - getStartStopLat(), getStartStopLon(), getEndStopLat(), getEndStopLon() zwracające double.

// Stała do przeliczenia stopni na radiany

// Funkcja obliczająca odległość według wzoru haversine (wynik w kilometrach)
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;
    lat1 = lat1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::sin(dLon / 2) * std::sin(dLon / 2) * std::cos(lat1) * std::cos(lat2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return 6371.0 * c; // Promień Ziemi w km
}

// Funkcja heurystyczna – szacuje czas przejazdu pomiędzy przystankami na podstawie odległości
// Zakładamy średnią prędkość 40 km/h (czyli czas w sekundach = odległość/40 * 3600)
std::chrono::system_clock::duration heuristic(const std::string &current,
                                                const std::string &target,
                                                const std::vector<edge> &edges) {
    // Pomocnicza funkcja zwracająca współrzędne przystanku (jeśli znajdzie pierwszy pasujący edge)
    auto getCoordinates = [&edges](const std::string &stop) -> std::pair<double, double> {
        for (const auto &e : edges) {
            if (e.getStartStop() == stop) {
                return {e.getStartStopLat(), e.getStartStopLon()};
            }
            if (e.getEndStop() == stop) {
                return {e.getEndStopLat(), e.getEndStopLon()};
            }
        }
        return {0.0, 0.0}; // Jeśli nie znaleziono, zwracamy domyślnie (0,0)
    };

    auto [lat1, lon1] = getCoordinates(current);
    auto [lat2, lon2] = getCoordinates(target);
    double distance = haversine(lat1, lon1, lat2, lon2); // w km
    double seconds = (distance / 30.0) * 3600.0;
    const double TRANSFER_PENALTY = 300.0; // 5 minut kary za przesiadkę

    std::unordered_set<std::string> current_lines;
    std::unordered_set<std::string> target_lines;

    for (const auto &e : edges) {
        if (e.getStartStop() == current || e.getEndStop() == current) current_lines.insert(e.getLine());
        if (e.getStartStop() == target || e.getEndStop() == target) target_lines.insert(e.getLine());
    }

    // Sprawdź przecięcie zbiorów linii
    bool shares_line = false;
    for (const auto &line : current_lines) {
        if (target_lines.count(line)) {
            shares_line = true;
            break;
        }
    }

    // Dodatkowa kara jeśli nie ma wspólnej linii
    if (!shares_line) {
        seconds += TRANSFER_PENALTY * 2; // 10 minut
    }

    return std::chrono::seconds(static_cast<int>(seconds));
}

bool AStarState::operator>(const AStarState &other) const {
    return estimated > other.estimated;
}



// Funkcja astar_time analogiczna do dijkstra_time, lecz używająca algorytmu A* z heurystyką.
std::pair<std::vector<edge>, double> astar_time(
    const std::unordered_map<std::string, std::vector<edge>>& adj,
    const std::vector<edge>& edges, // lista krawędzi potrzebna do obliczeń heurystyki
    const std::string &start,
    const std::string &end,
    const std::chrono::system_clock::time_point &startTime)
{
    // Najlepszy czas dojścia do danego przystanku
    std::unordered_map<std::string, std::chrono::system_clock::time_point> best;
    best[start] = startTime;

    std::priority_queue<AStarState, std::vector<AStarState>, std::greater<AStarState>> pq;

    // Inicjalny stan z heurystyką
    auto initial_heuristic = heuristic(start, end, edges);
    AStarState init{start, startTime, {}, startTime + initial_heuristic};
    pq.push(init);

    while (!pq.empty()) {
        AStarState current = pq.top();
        pq.pop();

        // Jeśli dotarliśmy do celu, obliczamy koszt (czas przejazdu w sekundach)
        if (current.stop == end) {
            double cost = std::chrono::duration_cast<std::chrono::seconds>(current.time - startTime).count();
            return {current.route, cost};
        }

        // Jeżeli dla danego przystanku nie mamy żadnych krawędzi, pomijamy iterację.
        if (adj.find(current.stop) == adj.end()) {
            continue;
        }

        for (const auto &e : adj.at(current.stop)) {
            // Wsiadamy tylko, gdy czas odjazdu nie jest wcześniejszy niż bieżący czas
            if (e.getDepartureTime() >= current.time) {
                auto arrival = e.getArrivalTime();
                // Aktualizujemy, jeśli dotarcie do e.getEndStop() nastąpi wcześniej
                if (best.find(e.getEndStop()) == best.end() || arrival < best[e.getEndStop()]) {
                    best[e.getEndStop()] = arrival;
                    std::vector<edge> newRoute = current.route;
                    newRoute.push_back(e);
                    auto h = heuristic(e.getEndStop(), end, edges);
                    AStarState next{e.getEndStop(), arrival, newRoute, arrival + h};
                    pq.push(next);
                }
            }
        }
    }

    // W przypadku braku znalezienia ścieżki – zwracamy pustą trasę oraz koszt -1 (sygnalizacja błędu)
    return {{}, -1.0};
}


//--

/// Funkcja astar_change szuka trasy o minimalnej liczbie przesiadek.
/// Warunkiem wejściowym jest zachowanie poprawności czasowej (nie możemy wsiąść przed aktualnym czasem).
/// Przy zmianie linii (porównujemy e.getLine() z ostatnią linią w trasie) zwiększamy koszt o 1.


std::pair<std::vector<edge>, double> astar_change(
    std::unordered_map<std::string, std::vector<edge>>& adj,
    const std::vector<edge>& edges, // lista krawędzi, opcjonalnie – przydatna przy heurystyce czy debugowaniu
    const std::string &start,
    const std::string &end,
    const std::chrono::system_clock::time_point &startTime)
{
    // Dla każdego przystanku przechowujemy najlepszy (minimalny) stan: liczba przesiadek oraz czas dojścia
    std::unordered_map<std::string, BestState> best;
    best[start] = {0, startTime};

    std::priority_queue<TransferState, std::vector<TransferState>, std::greater<TransferState>> pq;
    pq.push(TransferState{start, startTime, 0, {}, 0});

    while (!pq.empty()) {
        TransferState current = pq.top();
        pq.pop();

        if (current.stop == end) {
            return {current.route, current.transfers};
        }

        // Sprawdzenie, czy obecny stan jest nadal najlepszy
        auto currentBestIt = best.find(current.stop);
        if (currentBestIt != best.end()) {
            const BestState &currentBest = currentBestIt->second;
            if (current.transfers > currentBest.transfers ||
                (current.transfers == currentBest.transfers && current.time > currentBest.time)) {
                continue;
            }
        }

        // Opcja czekania (np. o 15 minut) na lepsze połączenie
        auto waitTime = current.time + std::chrono::minutes(15);
        bool hasEdgesAfterWait = false;
        if (adj.find(current.stop) != adj.end()) {
            for (const auto &e : adj.at(current.stop)) {
                if (e.getDepartureTime() >= waitTime) {
                    hasEdgesAfterWait = true;
                    break;
                }
            }
        }
        if (hasEdgesAfterWait) {
            TransferState waitState{
                current.stop,
                waitTime,
                current.transfers,
                current.route,
                current.transfers
            };

            auto &bestForStop = best[waitState.stop];
            if (bestForStop.transfers > waitState.transfers ||
                (bestForStop.transfers == waitState.transfers && bestForStop.time > waitState.time)) {
                bestForStop = {waitState.transfers, waitState.time};
                pq.push(waitState);
            }
        }

        if (adj.find(current.stop) == adj.end()) continue;

        for (const auto &e : adj.at(current.stop)) {
            if (e.getDepartureTime() >= current.time) {
                auto arrival = e.getArrivalTime();
                // Sprawdzenie zmiany linii – jeśli poprzednia linia (ostatni element trasy) różni się od bieżącej
                int additionalTransfer = (!current.route.empty() && current.route.back().getLine() != e.getLine()) ? 1 : 0;
                int newTransfers = current.transfers + additionalTransfer;

                bool updateBest = false;
                auto it = best.find(e.getEndStop());
                if (it == best.end()) {
                    best[e.getEndStop()] = {newTransfers, arrival};
                    updateBest = true;
                } else {
                    BestState &currentBest = it->second;
                    if (newTransfers < currentBest.transfers ||
                        (newTransfers == currentBest.transfers && arrival < currentBest.time)) {
                        currentBest.transfers = newTransfers;
                        currentBest.time = arrival;
                        updateBest = true;
                    }
                }

                if (updateBest) {
                    std::vector<edge> newRoute = current.route;
                    newRoute.push_back(e);
                    pq.push(TransferState{e.getEndStop(), arrival, newTransfers, newRoute, newTransfers});
                }
            }
        }
    }

    // W przypadku braku znalezienia ścieżki – zwracamy pustą trasę oraz koszt -1.
    return {{}, -1.0};
}


bool TransferState::operator>(const TransferState &other) const {
    if (estimated == other.estimated) {
        return time > other.time;
    }
    return estimated > other.estimated;
}
