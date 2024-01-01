#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int parseElo(std::string line) {
    std::string eloString = line.substr(line.find("\"") + 1, line.find_last_of("\"") - line.find("\"") - 1);
    if (eloString == "?") {
        return 0;
    }
    return std::stoi(eloString);
}

std::vector<std::string> filterPGN(const std::string& filename, int MIN_GAME_ELO) {
    std::vector<std::string> filteredGames;
    std::ifstream file(filename);
    std::string line, currentGame;
    int whiteElo = 0, blackElo = 0;
    int gameCount = 0, filteredGameCount = 0;
    bool foundMoves = false;

    while (getline(file, line)) {
        // Collecting lines for one game
        currentGame += line + "\n";

        if (line.find("[WhiteElo \"") != std::string::npos) {
            whiteElo = parseElo(line);
        } else if (line.find("[BlackElo \"") != std::string::npos) {
            blackElo = parseElo(line);
        }

        // Check for end of game and filter by ELO
        if (line == "" && foundMoves) {
            gameCount++;
            if (gameCount % 100000 == 0) {
                float filteredPct = static_cast<float>(filteredGameCount) / gameCount * 100;
                std::cout << "Total Games: " << gameCount << " - Filtered Games: " << filteredGameCount << " - Filtered Pct: " << filteredPct << "%" << std::endl;
            }
            if (std::min(whiteElo, blackElo) >= MIN_GAME_ELO) {
                filteredGames.push_back(currentGame);
                ++filteredGameCount;
            }
            currentGame.clear();
            foundMoves = false;
        } else if (line[0] != '[') {
            foundMoves = true;
        }
    }

    return filteredGames;
}

int main() {
    std::string inputPath = "lichess_games.pgn";
    std::string outputPath = "filtered_games.pgn";
    int minElo = 2000;  // Approximately top 3.71%
    std::vector<std::string> filteredGames = filterPGN(inputPath, minElo);

    // Write filtered games to output
    std::ofstream outputFile(outputPath);
    for (const std::string& pgn : filteredGames) {
        outputFile << pgn;  // No need for adding ending lines because the pgn object already contains ending lines
    }
    outputFile.close();

    return 0;
}
