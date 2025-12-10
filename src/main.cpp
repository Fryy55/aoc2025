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
		bool right = in.starts_with('R');
		auto numOpt = utils::numFromStr<std::uint64_t>(std::string_view(in).substr(1u));
		if (!numOpt.has_value()) {
			log::error("Failed parsing the string '{}'", in);
			return 1;
		}
		auto num = numOpt.value();

		if (right) {
			if (auto diff = 100u - dial; diff <= num) // if an overflow occurs
				dial = (num - diff) % 100u;
			else
				dial += num;
		} else { // left
			if (dial < num) // if an overflow occurs
				dial = (100u - (num - dial) % 100u) % 100u;
			else
				dial -= num;
		}

		if (dial == 0u)
			++ans;
	}

	I.close();

	log::info("Answer: {}", ans);

	return 0;
}