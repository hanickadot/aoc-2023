#include <ctre.hpp>
#include <aoc/support.hpp>
#include <iostream>
#include <ranges>
#include <numeric>
#include <cassert>
#include <algorithm>

struct accumulator {
	struct pair {
		unsigned first;
		unsigned last{0};
		
		constexpr pair(unsigned f) noexcept: first{f} { }
		constexpr pair(unsigned f, unsigned l) noexcept: first{f}, last{l} { }
		
		friend constexpr pair operator+(pair lhs, unsigned rhs) {
			return pair{lhs.first, rhs};
		}
	};
	
	unsigned sum{0};
	std::optional<pair> current_line{std::nullopt};
	
	unsigned finish_line() const noexcept {
		const auto value_of_current_line = current_line.transform([](pair p) { return p.first * 10u + p.last; }).value_or(0);
		return sum + value_of_current_line;
	}
	
	static auto get_word_digit_as_number(const ctre::capture_groups auto & captures) {
		// we look only for numbers 1..9
		return [&]<size_t... Is>(std::index_sequence<Is...>, unsigned result = 0) {
			// equivalent of if(captures.get<N + 2u>) { result = N + 1; } else...
			((bool(captures.template get<Is + 3u>()) && (result = (Is + 1u))) || ...);
			return result;
		}(std::make_index_sequence<9u>());
	}
	
	static auto get_digit(const ctre::capture_groups auto & captures) {
		// numerical digits
		if (captures.template get<"digit">()) {
			return captures.template to_number<unsigned>();
		}
		
		// digits as words
		return get_word_digit_as_number(captures);
	}
	
	friend accumulator operator+(const accumulator & lhs, const ctre::capture_groups auto & captures) noexcept {
		// we reached end of line
		if (captures.template get<"newline">()) {
			return accumulator{
				.sum = lhs.finish_line()
			};
		}
		
		const auto current_digit = get_digit(captures);
		
		// just store current digit as it's possible last
		return accumulator{
			.sum = lhs.sum,
			.current_line = lhs.current_line.value_or(current_digit) + current_digit
		};
	}

};

auto calculate_result(std::string_view line) -> unsigned {
	const auto occurences = ctre::search_all<
		"(?<newline>\r?\n)|"
		"(?<digit>[0-9])|"
		"(o(?=ne))|"
		"(t(?=wo))|"
		"(t(?=hree))|"
		"(f(?=our))|"
		"(f(?=ive))|"
		"(s(?=ix))|"
		"(s(?=even))|"
		"(e(?=ight))|"
		"(n(?=ine))"
	>(line);
	
	return std::ranges::fold_left(occurences, accumulator{}, std::plus{}).finish_line();
}

int main(int argc, char ** argv) {
	if (argc < 2) {
		return 1;
	}
	
	std::cout << calculate_result(aoc::read_file(argv[1])) << "\n";
}