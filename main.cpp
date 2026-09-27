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
    constexpr std::array<Position, 6> startingPositions{{
        {0, 0},
        {0, height - 1},
        {(width - 1) / 2, (height - 1) / 2},
        {width / 2, height / 2},
        {width - 1, 0},
        {width - 1, height - 1}
    }};

    struct Algorithm {
        const char* name;
        Path (Board::*search)(Position start);
    };
    constexpr std::array<Algorithm, 4> algorithms{{
        {"BFS", &Board::bfs},
        {"DFS", &Board::dfs},
        {"IDS", &Board::ids},
        {"AStar", &Board::aStar}
    }};

    const std::filesystem::path outputDirectory = "results";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Board: " << width << 'x' << height << '\n';

    for (const auto& algorithm : algorithms) {
        for (const auto start : startingPositions) {
            std::cout << algorithm.name << " start=(" << start.x << ", " << start.y << "): ";
            try {
                Board board(width, height);
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
                    (std::string(algorithm.name) + "_" + std::to_string(width) + "x" +
                     std::to_string(height) + "_" + std::to_string(start.x) + "_" +
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
