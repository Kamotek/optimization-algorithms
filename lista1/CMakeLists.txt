
cmake_minimum_required(VERSION 3.30)
project(lista1)

set(CMAKE_CXX_STANDARD 20)

add_executable(lista1 src/main.cpp
        src/graph_utils/csv_reader.cpp
        src/graph_utils/csv_reader.h
        src/graph_utils/graph_generator.cpp
        src/graph_utils/graph_generator.h
        src/graph_utils/graph.cpp
        src/graph_utils/graph.h
        src/ui/user_cli.cpp
        src/ui/user_cli.h)
