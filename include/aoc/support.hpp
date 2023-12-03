#ifndef AOC_AOC_SUPPORT_HPP
#define AOC_AOC_SUPPORT_HPP

#include <filesystem>
#include <string>

namespace aoc {

[[nodiscard]] std::string read_file(const std::filesystem::path & path);

}

#endif
