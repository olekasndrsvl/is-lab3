#include "board.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace {
    constexpr std::size_t maxStates = 1'000'000'000;
    constexpr std::size_t noParent = std::numeric_limits<std::size_t>::max();

    constexpr std::array<Position, 8> moves{{
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    }};

    struct State {
        Board board;
        Position position;
        std::size_t parent;
    };
}

Path Board::bfs(Position start) {
    if (!isInside(start.x, start.y)) {
        throw std::out_of_range("Coordinates are outside the board");
    }

    Board initialBoard(_width, _height);
    initialBoard.markVisited(start.x, start.y);

    std::vector<State> states;
    states.push_back({initialBoard, start, noParent});

    for (std::size_t head = 0; head < states.size(); ++head) {
        const State current = states[head];

        if (current.board.isComplete()) {
            Path path;
            path.reserve(cellCount());

            std::size_t index = head;
            while (index != noParent) {
                path.push_back(states[index].position);
                index = states[index].parent;
            }

            std::reverse(path.begin(), path.end());
            _field = current.board._field;
            return path;
        }

        for (const auto move : moves) {
            const Position next{
                current.position.x + move.x,
                current.position.y + move.y
            };
            if (!isInside(next.x, next.y) || current.board.isVisited(next.x, next.y)) {
                continue;
            }

            // контрим если все очень плохо
            if (states.size() >= maxStates) {
                throw std::runtime_error("BFS state limit reached (" + std::to_string(maxStates) + ")");
            }

            Board nextBoard = current.board;
            nextBoard.markVisited(next.x, next.y);
            states.push_back({nextBoard, next, head});
        }
    }

    return {};
}
