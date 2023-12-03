#include "support.hpp"
#include <fstream>
#include <stdexcept>
#include <cassert>

auto aoc::read_file(const std::filesystem::path & path) -> std::string {
	auto i = std::ifstream{path, std::ios::binary};
	
	if (!i) {
		throw std::invalid_argument{"cannot open file!"};
	}
	
	i.seekg(0, std::ios_base::end);
	
	const size_t size = i.tellg();
	
	i.seekg(0, std::ios_base::beg);
	
	auto output = std::string{};
	output.reserve(size);
	
	output.assign((std::istreambuf_iterator<char>(i)), std::istreambuf_iterator<char>());
	
	assert(output.size() == size);
	
	return output;
}

