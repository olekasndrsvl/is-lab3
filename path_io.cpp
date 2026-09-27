#include "board.h"

#include <fstream>
#include <stdexcept>

void Board::writePathToFile(const Path& path, const std::filesystem::path& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filename.string());
    }

    file << "# Coordinates x and y are zero-based (0..7).\n";
    file << "step x y\n";
    for (std::size_t i = 0; i < path.size(); ++i) {
        file << i + 1 << ' ' << path[i].x << ' ' << path[i].y << '\n';
    }

    file.close();
    if (!file) {
        throw std::runtime_error("Cannot write output file: " + filename.string());
    }
}
