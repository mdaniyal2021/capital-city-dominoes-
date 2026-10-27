#include "Simulator.h"
#include "Prototype.h"
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <list>
#include <string>
#include <cstddef>
#include <vector>
#include <mutex>
#include <functional>

namespace
{
    constexpr int alphabetSize = 26;
    constexpr unsigned int defaultThreadCount = 4;
}

std::list<std::string> Simulator::run(const std::string& file, char start, int seed)
{
    Prototype game(file);
    game.seed(seed);

    std::list<std::string> playedWords;
    char currentLetter = start;

    while (true)
    {
        std::string city = game.getCity(currentLetter);
        if (city.empty())
        {
            break;
        }

        playedWords.push_back(city);
        game.markUsed(city);
        currentLetter = city.back();
    }

    return playedWords;
}

void Simulator::generateBatchParameters(int k, int seed,
    std::vector<char>& startLetters, std::vector<int>& runSeeds)
{
    std::mt19937 batchRng(static_cast<unsigned int>(seed));
    std::uniform_int_distribution<int> letterDist(0, alphabetSize - 1);

    startLetters.resize(static_cast<size_t>(k));
    runSeeds.resize(static_cast<size_t>(k));

    for (int i = 0; i < k; ++i)
    {
        startLetters.at(static_cast<size_t>(i)) = static_cast<char>('a' + letterDist(batchRng));
        runSeeds.at(static_cast<size_t>(i)) = static_cast<int>(batchRng());
    }
}

unsigned int Simulator::determineThreadCount(int k)
{
    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0)
    {
        threadCount = defaultThreadCount;
    }
    threadCount = static_cast<unsigned int>(std::min<int>(static_cast<int>(threadCount), k));
    if (threadCount == 0)
    {
        threadCount = 1;
    }
    return threadCount;
}

size_t Simulator::reserveResultSlots(int k)
{
    std::lock_guard<std::mutex> lock(resultsMutex);
    size_t baseIndex = results.size();
    results.resize(results.size() + static_cast<size_t>(k));
    return baseIndex;
}

void Simulator::runRange(const std::string& file, const std::vector<char>& startLetters,
    const std::vector<int>& runSeeds, size_t baseIndex, int rangeStart, int rangeEnd)
{
    for (int i = rangeStart; i < rangeEnd; ++i)
    {
        std::list<std::string> result = run(file, startLetters.at(static_cast<size_t>(i)), runSeeds.at(static_cast<size_t>(i)));
        results.at(baseIndex + static_cast<size_t>(i)) = result;
    }
}

void Simulator::dispatchWorkers(const std::string& file, const std::vector<char>& startLetters,
    const std::vector<int>& runSeeds, size_t baseIndex, int k)
{
    unsigned int threadCount = determineThreadCount(k);

    std::vector<std::thread> workers;
    int chunkSize = k / static_cast<int>(threadCount);
    int remainder = k % static_cast<int>(threadCount);
    int rangeStart = 0;

    for (unsigned int t = 0; t < threadCount; ++t)
    {
        int rangeSize = chunkSize + (static_cast<int>(t) < remainder ? 1 : 0);
        int rangeEnd = rangeStart + rangeSize;

        workers.emplace_back(&Simulator::runRange, this, std::cref(file), std::cref(startLetters), std::cref(runSeeds), baseIndex, rangeStart, rangeEnd);

        rangeStart = rangeEnd;
    }

    for (auto& worker : workers)
    {
        worker.join();
    }
}

double Simulator::batch(const std::string& file, int k, int seed)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<char> startLetters;
    std::vector<int> runSeeds;
    generateBatchParameters(k, seed, startLetters, runSeeds);

    size_t baseIndex = reserveResultSlots(k);

    dispatchWorkers(file, startLetters, runSeeds, baseIndex, k);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    return elapsed.count();
}

std::vector<std::list<std::string>> Simulator::getResults()
{
    std::lock_guard<std::mutex> lock(resultsMutex);
    return results;
}