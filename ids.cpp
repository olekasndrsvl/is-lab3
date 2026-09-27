#include "board.h"

#include <stdexcept>

Path Board::ids(Position start) {
    if (!isInside(start.x, start.y)) {
        throw std::out_of_range("Coordinates are outside the board");
    }

    Board searchBoard(_width, _height);
    Path path;
    path.reserve(cellCount());

    for (int depthLimit = 0; depthLimit < cellCount(); ++depthLimit) {
        searchBoard.reset();
        searchBoard.markVisited(start.x, start.y);
        path.clear();
        path.push_back(start);

        if (searchBoard.depthLimitedSearch(start, depthLimit, path)) {
            _field = searchBoard._field;
            return path;
        }
    }

    return {};
}
