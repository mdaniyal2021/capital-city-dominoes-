#pragma once
#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <string>
#include <unordered_set>
#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <cstddef>

class Prototype {
private:
    std::unordered_set<std::string> cities;
    std::unordered_set<std::string> usedCities;
    std::mt19937 rng;

protected:
    // Read-only access to the full city list, for subclasses implementing
    // custom selection strategies (e.g. Competition).
    const std::unordered_set<std::string>& getAllCities() const { return cities; }

    // Read-only access to the set of cities already played, for subclasses
    // implementing custom selection strategies.
    const std::unordered_set<std::string>& getUsedCities() const { return usedCities; }

public:

    Prototype(const std::string& filename);


    std::string getCity(char c);


    bool checkCity(const std::string& city);

    void markUsed(const std::string& city);


    void restart();

    void seed(int s);
};

#endif