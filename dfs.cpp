#include "board.h"

#include <array>
#include <stdexcept>

namespace {
    constexpr std::array<Position, 8> moves{{
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    }};
}

bool Board::depthLimitedSearch(Position current, int remainingDepth, Path& path) {
    if (isComplete()) {
        return true;
    }
    if (remainingDepth == 0) {
        return false;
    }

    for (const auto move : moves) {
        const Position next{current.x + move.x, current.y + move.y};
        if (!isInside(next.x, next.y) || isVisited(next.x, next.y)) {
            continue;
        }

        markVisited(next.x, next.y);
        path.push_back(next);
        if (depthLimitedSearch(next, remainingDepth - 1, path)) {
            return true;
        }

        path.pop_back();
        unmarkVisited(next.x, next.y);
    }

    return false;
}

Path Board::dfs(Position start) {
    if (!isInside(start.x, start.y)) {
        throw std::out_of_range("Coordinates are outside the board");
    }

    Board searchBoard(_width, _height);
    searchBoard.markVisited(start.x, start.y);
    Path path;
    path.reserve(cellCount());
    path.push_back(start);

    if (searchBoard.depthLimitedSearch(start, cellCount() - 1, path)) {
        _field = searchBoard._field;
        return path;
    }
    return {};
}
