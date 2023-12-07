#include <ctre.hpp>
#include <aoc/support.hpp>
#include <iostream>
#include <ranges>
#include <numeric>
#include <cassert>
#include <algorithm>

struct game {
	unsigned id;
	
	game(const ctre::capture_groups auto & line): 
		id{line.template get<"id">().template to_number<unsigned>()}
	{ }
};

struct colors {
	unsigned red = 0;
	unsigned green = 0;
	unsigned blue = 0;
	
	constexpr bool is_possible(const colors & limit) const noexcept {
		return (red <= limit.red) && (green <= limit.green) && (blue <= limit.blue);
	}
	constexpr friend colors max(const colors & a, const colors & b) noexcept {
		return {
			.red = std::max(a.red, b.red),
			.green = std::max(a.green, b.green),
			.blue = std::max(a.blue, b.blue)
		};
	}
	constexpr unsigned multiply_together() const noexcept {
		return red * green * blue;
	}
};

auto create_game(const ctre::capture_groups auto & line, const colors & limit = {}) -> std::optional<unsigned> {
	auto && [_1, _id, sets] = line;
	
	const auto id = _id.template to_number<unsigned>();
	
	auto count = colors{};
	auto least = colors{};
	
	for (auto dice: sets | ctre::tokenize<"([0-9]+) (?:(red)|(green)|(blue))(?:, |(; |$))">) {
		auto && [_2, _count, red, green, blue, last] = dice;
		const auto individual_count = _count.template to_number<unsigned>();
		
		if (red) {
			count.red = individual_count;
		} else if (green) {
			count.green = individual_count;
		} else {
			assert(blue);
			count.blue = individual_count;
		}
		
		if (last) {
			least = max(least, count);
			//if (!count.is_possible(limit)) {
			//	return std::nullopt;
			//}
			count = {};
		}
	}
	
	return least.multiply_together();
	//return id;
}

unsigned calculate_result(std::string_view content, const colors & limit = {}) {
	const auto get_each_line = ctre::tokenize<"Game (?<id>[0-9]+): (?<game>[^\r\n]++)\r?\n?">;
	const auto transform_to_game_result = std::views::transform([=](const ctre::capture_groups auto & line) { return create_game(line, limit); });
	const auto only_valid = std::views::filter([](const auto & value) { return value.has_value(); });
	const auto unwrap = std::views::transform([](auto && value) { return *value; });

	return std::ranges::fold_left(get_each_line(content) | transform_to_game_result | only_valid | unwrap, 0u, std::plus<>{});
}

int main(int argc, char ** argv) {
	if (argc < 2) {
		return 1;
	}
	
	const auto r = calculate_result(aoc::read_file(argv[1]), {.red = 12, .green = 13, .blue = 14});
	std::cout << "result = " << r << "\n";
}