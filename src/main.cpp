#include <aurora/aurora.hpp>

#include "prelude.hpp"
#include "num-str.hpp"

#include <fstream>


int main() {
	AOC_PRELUDE

	std::uint8_t dial = 50u;
	std::uint64_t ans = 0u;

	std::string in;
	while (I >> in) {
		bool left = in.starts_with('L');
		auto numOpt = utils::numFromStr<std::uint64_t>(std::string_view(in).substr(1u));
		if (!numOpt.has_value()) {
			log::error("Failed parsing the string '{}'", in);
			return 1;
		}
		auto num = numOpt.value();

		if (left) {
			dial = 100u - dial;
		}

		// right move
		if (auto diff = 100u - dial; diff <= num) { // if an overflow occurs
			dial = (num - diff) % 100u;
			ans += (num - diff) / 100u + 1u;

			if (left && diff == 0u) // :p
				--ans;
		} else {
			dial += num;

			if (dial == 0u)
				++ans;
		}

		if (left)
			dial = (100u - dial) % 100u;
	}

	I.close();

	log::info("Answer: {}", ans);

	return 0;
}