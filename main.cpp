#include "board.h"

#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>

int main() {
    constexpr int width = 3;
    constexpr int height = 4;
    struct Algorithm {
        const char* name;
        Path (Board::*search)(Position start);
        int width;
        int height;
    };
    constexpr std::array<Algorithm, 5> algorithms{{
        {"BFS", &Board::bfs, width, height},
        {"DFS", &Board::dfs, width, height},
        {"IDS", &Board::ids, width, height},
        {"AStar", &Board::aStar, width, height},
        {"AStar", &Board::aStar, 8, 8}
    }};

    const std::filesystem::path outputDirectory = "results";
    std::cout << std::fixed << std::setprecision(3);

    for (const auto& algorithm : algorithms) {
        const int boardWidth = algorithm.width;
        const int boardHeight = algorithm.height;
        const std::array<Position, 6> startingPositions{{
            {0, 0},
            {0, boardHeight - 1},
            {(boardWidth - 1) / 2, (boardHeight - 1) / 2},
            {boardWidth / 2, boardHeight / 2},
            {boardWidth - 1, 0},
            {boardWidth - 1, boardHeight - 1}
        }};
        std::cout << "Board: " << boardWidth << 'x' << boardHeight << '\n';
        for (const auto start : startingPositions) {
            std::cout << algorithm.name << " start=(" << start.x << ", " << start.y << "): ";
            try {
                Board board(boardWidth, boardHeight);
                const auto begin = std::chrono::steady_clock::now();
                const Path path = (board.*algorithm.search)(start);
                const auto end = std::chrono::steady_clock::now();
                const double elapsedMs = std::chrono::duration<double, std::milli>(end - begin).count();

                std::cout << elapsedMs << " ms";
                if (path.empty()) {
                    std::cout << ", no tour found\n";
                    continue;
                }

                const auto filename = outputDirectory /
                    (std::string(algorithm.name) + "_" + std::to_string(boardWidth) + "x" +
                     std::to_string(boardHeight) + "_" + std::to_string(start.x) + "_" +
                     std::to_string(start.y) + ".txt");
                std::filesystem::create_directories(outputDirectory);
                board.writePathToFile(path, filename);
                std::cout << ", " << path.size() << " positions, saved to "
                          << filename.string() << '\n';
            } catch (const std::exception& error) {
                std::cout << "error: " << error.what() << '\n';
            }
        }
    }
    return 0;
}
