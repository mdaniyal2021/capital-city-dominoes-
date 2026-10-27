#pragma once
#include "Prototype.h"

#define Competition_class Competition			// now using our implemented Competition class

class Competition : public Prototype
{
public:
    // Inherit Prototype's constructor (takes the word-list filename)
    using Prototype::Prototype;

    // Overrides the base random selection with a heuristic strategy:
    // prefer moves that leave the opponent with the fewest valid replies.
    std::string getCity(char c);

private:
    // Counts how many currently-unused cities start with the given letter.
    // Used to estimate how restrictive a candidate move is for the opponent.
    int countAvailableStarts(char letter) const;
};