
cmake_minimum_required(VERSION 3.30)
project(lista1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lista1 src/main.cpp
        src/io_handling/csv_reader.cpp
        src/io_handling/csv_reader.h
        src/graph/graph_generator.cpp
        src/graph/graph_generator.h
        src/graph/edge.cpp
        src/graph/edge.h
        src/ui/user_cli.cpp
        src/ui/user_cli.h
        src/algorithms_utils/time_calc.cpp
        src/algorithms_utils/time_calc.h
        src/algorithms/astar.cpp
        src/algorithms/astar.h
        src/algorithms/dijkstra.cpp
        src/algorithms/dijkstra.h
        src/graph/vertex.cpp
        src/graph/vertex.h
        src/graph/graph.cpp
        src/graph/graph.h
        src/algorithms/tabu_search.cpp
        src/algorithms/tabu_search.h
        src/algorithms/tabu_search_knox.cpp
        src/algorithms/tabu_search_knox.h)
