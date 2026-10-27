#include "Prototype.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstddef>

Prototype::Prototype(const std::string& filename) : rng(std::random_device{}()) {
    std::ifstream file(filename);
    std::string city;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return;
    }


    while (std::getline(file, city)) {
        cities.insert(city);
    }
    file.close();
}


std::string Prototype::getCity(char c) {
    std::vector<std::string> candidates;

    for (const auto& city : cities) {
        if (!city.empty() && city.front() == c && !usedCities.contains(city)) {
            candidates.push_back(city);
        }
    }

    if (candidates.empty()) {
        return "";
    }

    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidates.at(dist(rng));
}


bool Prototype::checkCity(const std::string& city) {
    return cities.contains(city) && !usedCities.contains(city);
}


void Prototype::markUsed(const std::string& city) {
    if (cities.contains(city)) {
        usedCities.insert(city);
    }
}


void Prototype::restart() {
    usedCities.clear();
}

void Prototype::seed(int s) {
    rng.seed(static_cast<unsigned int>(s));
}