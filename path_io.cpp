#include "board.h"

#include <fstream>
#include <stdexcept>

void Board::writePathToFile(const Path& path, const std::filesystem::path& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filename.string());
    }

    file << "# Board: " << _width << 'x' << _height << '\n';
    file << "# Zero-based coordinates: x in [0, " << _width - 1
         << "], y in [0, " << _height - 1 << "].\n";
    file << "step x y\n";
    for (std::size_t i = 0; i < path.size(); ++i) {
        file << i + 1 << ' ' << path[i].x << ' ' << path[i].y << '\n';
    }

    file.close();
    if (!file) {
        throw std::runtime_error("Cannot write output file: " + filename.string());
    }
}
