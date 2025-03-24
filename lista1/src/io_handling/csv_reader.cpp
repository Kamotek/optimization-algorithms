//
// Created by kamil on 19.03.2025.
//
#include "csv_reader.h"

#include <iostream>


std::vector<std::string> readCSVFile(const std::string& fileName) {
std::vector<std::string> resultData;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "File " << fileName << " does not exist." << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token)) {
            row.push_back(token);
        }
        resultData.push_back(token);


    }

    file.close();
    return resultData;
}

