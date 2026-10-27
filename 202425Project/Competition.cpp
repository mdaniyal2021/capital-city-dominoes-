#include "Competition.h"
#include <vector>
#include <algorithm>
#include <string>

int Competition::countAvailableStarts(char letter) const
{
    return static_cast<int>(std::count_if(getAllCities().begin(), getAllCities().end(),
        [this, letter](const std::string& city)
        {
            return !city.empty() && city.front() == letter && !getUsedCities().contains(city);
        }));
}

std::string Competition::getCity(char c)
{
    std::vector<std::string> candidates;
    for (const auto& city : getAllCities())
    {
        if (!city.empty() && city.front() == c && !getUsedCities().contains(city))
        {
            candidates.push_back(city);
        }
    }

    if (candidates.empty())
    {
        return "";
    }

    // Score each candidate by how many cities would remain available to
    // the opponent on their next turn (based on this candidate's last
    // letter). Fewer remaining options for the opponent is better for us,
    // since it increases the chance they get stuck with no valid reply.
    std::string bestCity;
    int fewestOptionsForOpponent = -1;

    for (const auto& city : candidates)
    {
        char nextLetter = city.back();
        int remaining = countAvailableStarts(nextLetter);

        if (c == nextLetter)
        {
            remaining -= 1;
        }

        if (fewestOptionsForOpponent == -1 || remaining < fewestOptionsForOpponent)
        {
            fewestOptionsForOpponent = remaining;
            bestCity = city;
        }
    }

    return bestCity;
}