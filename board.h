#pragma once

#include <cstdint>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <vector>

struct Position {
    int x; // [0..width-1]
    int y; // [0..height-1]
};

using Path = std::vector<Position>;

class Board {
public:
    explicit Board(int width = 8, int height = 8)
        : _width(width), _height(height) {
        if (width < 1 || width > 8 || height < 1 || height > 8) {
            throw std::invalid_argument("Board dimensions must be in [1, 8]");
        }
    }

    inline int width() const noexcept {
        return _width;
    }

    inline int height() const noexcept {
        return _height;
    }

    inline int cellCount() const noexcept {
        return _width * _height;
    }


    Path bfs(Position start);
    Path dfs(Position start);
    Path ids(Position start);
    Path aStar(Position start);

    void writePathToFile(const Path& path, const std::filesystem::path& filename) const;

    inline bool isInside(int x, int y) const noexcept {
        return x >= 0 && x < _width && y >= 0 && y < _height;
    }

    inline void markVisited(int x, int y) {
        _field |= mask(x, y);
    }

    inline void unmarkVisited(int x, int y) {
        _field &= ~mask(x, y);
    }

    inline bool isVisited(int x, int y) const {
        return (_field & mask(x, y)) != 0;
    }

    inline bool isComplete() const noexcept {
        const auto fullMask = std::numeric_limits<std::uint64_t>::max()
                              >> (64 - cellCount());
        return _field == fullMask;
    }

    inline void reset() noexcept {
        _field = 0;
    }

private:
    int _width;
    int _height;
    std::uint64_t _field = 0;

    inline std::uint64_t mask(int x, int y) const {
        if (!isInside(x, y)) {
            throw std::out_of_range("Coordinates are outside the board");
        }
        return std::uint64_t{1} << (y * _width + x);
    }
};
