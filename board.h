#pragma once

#include <cstdint>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <vector>

struct Position {
    int x; // [0..7]
    int y; // обойдемся без сложностей с буквами [0..7]
};

using Path = std::vector<Position>;

class Board {
public:
    static constexpr int size = 8;
    static constexpr int cellCount = size * size;

    Path bfs(Position start);
    Path dfs(Position start);
    Path ids(Position start);
    Path aStar(Position start);

    void writePathToFile(const Path& path, const std::filesystem::path& filename) const;

    static inline bool isInside(int x, int y) noexcept {
        return x >= 0 && x < size && y >= 0 && y < size;
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
        return _field == std::numeric_limits<std::uint64_t>::max();
    }

    inline void reset() noexcept {
        _field = 0;
    }

private:
    std::uint64_t _field = 0;

    static inline std::uint64_t mask(int x, int y) {
        if (!isInside(x, y)) {
            throw std::out_of_range("Board coordinates must be in [0, 7]");
        }
        return std::uint64_t{1} << (y * size + x);
    }
};
