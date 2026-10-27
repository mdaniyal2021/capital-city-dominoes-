#pragma once
#include "Prototype.h"
#include <list>
#include <string>
#include <vector>
#include <mutex>
#include <cstddef>

class Simulator
{
private:
    // Stores the word sequence for every simulation executed so far
    std::vector<std::list<std::string>> results;

    // Protects 'results' when appending across batch() calls
    std::mutex resultsMutex;

    // Generates a deterministic sequence of starting letters and per-run
    // seeds for a batch, based on a single batch-level seed.
    static void generateBatchParameters(int k, int seed,
        std::vector<char>& startLetters, std::vector<int>& runSeeds);

    // Decides how many worker threads to use for a batch of size k.
    static unsigned int determineThreadCount(int k);

    // Reserves k new slots at the end of 'results' for this batch and
    // returns the index of the first reserved slot.
    size_t reserveResultSlots(int k);

    // Runs simulations for the index range [rangeStart, rangeEnd) and
    // writes each result into its reserved slot in 'results'.
    void runRange(const std::string& file, const std::vector<char>& startLetters,
        const std::vector<int>& runSeeds, size_t baseIndex, int rangeStart, int rangeEnd);

    // Splits k simulations across worker threads and waits for them
    // all to complete.
    void dispatchWorkers(const std::string& file, const std::vector<char>& startLetters,
        const std::vector<int>& runSeeds, size_t baseIndex, int k);

public:
    // Plays the prototype against itself using the given word file,
    // starting letter, and seed. Returns the sequence of words played.
    static std::list<std::string> run(const std::string& file, char start, int seed);

    // Runs k simulations concurrently across multiple threads, storing
    // each result internally. Returns the total time taken (in seconds).
    double batch(const std::string& file, int k, int seed);

    // Returns all simulation results executed so far, in execution order.
    std::vector<std::list<std::string>> getResults();
};