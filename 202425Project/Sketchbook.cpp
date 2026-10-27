#include "Competition.h"
#include "Prototype.h"
#include <iostream>

int main() {
    int competitionWins = 0;
    int totalGames = 200;

    for (int trial = 0; trial < totalGames; ++trial) {
        Competition competitionPlayer("Cities.txt");
        Prototype basicPlayer("Cities.txt");
        competitionPlayer.seed(trial);
        basicPlayer.seed(trial + 1000);

        char currentLetter = static_cast<char>('a' + (trial % 26));
        bool competitionTurn = true;
        bool competitionWon = false;

        for (int move = 0; move < 200; ++move) {
            std::string city;
            if (competitionTurn) {
                city = competitionPlayer.getCity(currentLetter);
            }
            else {
                city = basicPlayer.getCity(currentLetter);
            }

            if (city.empty()) {
                // Current player has no valid move, so they lose
                competitionWon = !competitionTurn;
                break;
            }

            competitionPlayer.markUsed(city);
            basicPlayer.markUsed(city);
            currentLetter = city.back();
            competitionTurn = !competitionTurn;
        }

        if (competitionWon) {
            competitionWins++;
        }
    }

    std::cout << "Competition won " << competitionWins << " out of " << totalGames << " games" << std::endl;
    std::cout << "Win rate: " << (100.0 * competitionWins / totalGames) << "%" << std::endl;

    return 0;
}