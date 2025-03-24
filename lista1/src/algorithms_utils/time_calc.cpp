//
// Created by kamil on 23.03.2025.
//

#include "time_calc.h"

#include <chrono>
#include <iostream>

#include "../graph/edge.h"

long calculate_time(const edge& start, const edge& end) {
    auto diff = end.getArrivalTime() - start.getDepartureTime();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff);
    return seconds.count();
}
