#include "board.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <queue>
#include <stdexcept>
#include <string>

namespace {
    constexpr std::size_t maxStates = 1'000'000;
    constexpr std::size_t noParent = std::numeric_limits<std::size_t>::max();

    constexpr std::array<Position, 8> moves{{
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    }};

    struct State {
        Board board;
        Position position;
        std::size_t parent;
        int g;
    };

    struct QueueEntry {
        std::size_t index;
        int f;
        int h;
        int degree;
    };

    struct LowerPriority {
        bool operator()(const QueueEntry& left, const QueueEntry& right) const {
            if (left.f != right.f) {
                return left.f > right.f;
            }

            if (left.h != right.h) {
                return left.h > right.h;
            }

            if (left.degree != right.degree) {
                return left.degree > right.degree;
            }
            return left.index > right.index;
        }
    };

    int countOnwardMoves(const Board& board, Position position) {
        int count = 0;
        for (const auto move : moves) {
            const Position next{position.x + move.x, position.y + move.y};
            if (board.isInside(next.x, next.y) && !board.isVisited(next.x, next.y)) {
                ++count;
            }
        }
        return count;
    }
}

Path Board::aStar(Position start) {
    if (!isInside(start.x, start.y)) {
        throw std::out_of_range("Coordinates are outside the board");
    }

    Board initialBoard(_width, _height);
    initialBoard.markVisited(start.x, start.y);
    std::vector<State> states;
    states.push_back({initialBoard, start, noParent, 0});


    const int initialH = cellCount() - 1;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, LowerPriority> open;
    open.push({0, initialH, initialH, countOnwardMoves(initialBoard, start)});

    while (!open.empty()) {
        const auto entry = open.top();
        open.pop();


        const State current = states[entry.index];

        if (current.board.isComplete()) {
            Path path;
            path.reserve(cellCount());
            std::size_t index = entry.index;
            while (index != noParent) {
                path.push_back(states[index].position);
                index = states[index].parent;
            }
            std::reverse(path.begin(), path.end());
            _field = current.board._field;
            return path;
        }

        for (const auto move : moves) {
            const Position next{current.position.x + move.x, current.position.y + move.y};
            if (!isInside(next.x, next.y) || current.board.isVisited(next.x, next.y)) {
                continue;
            }

            Board nextBoard = current.board;
            nextBoard.markVisited(next.x, next.y);
            const int g = current.g + 1;
            const int h = cellCount() - 1 - g;
            const int degree = countOnwardMoves(nextBoard, next);
            if (degree == 0 && h > 0) {
                continue;
            }

            if (states.size() >= maxStates) {
                throw std::runtime_error("A* state limit reached (" + std::to_string(maxStates) + ")");
            }
            const std::size_t index = states.size();
            states.push_back({nextBoard, next, entry.index, g});
            open.push({index, g + h, h, degree});
        }
    }

    return {};
}
