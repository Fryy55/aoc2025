#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <immintrin.h>
#include <optional>
#include <cstring>
#include <ranges>
#include <algorithm>


namespace utils {

template <std::integral T>
std::string numToStr(T num) noexcept {
	if (num == 0)
		return "0";

	alignas(32u) std::array<char, 32u> sharedBuffer;
	auto bufPtr = sharedBuffer.data();
	std::uint8_t digitsCount = 0u;

	bool negative = num < 0;
	if (negative)
		num = std::abs(num);

	for (; num != 0; ++digitsCount) {
		sharedBuffer[sharedBuffer.size() - digitsCount - 1u] = num % 10;
		num /= 10;
	}

	_mm256_store_si256(
		reinterpret_cast<__m256i*>(bufPtr),
		_mm256_add_epi8(
			_mm256_load_si256(reinterpret_cast<__m256i*>(bufPtr)),
			_mm256_set1_epi8('0')
		)
	);

	if (negative)
		sharedBuffer[sharedBuffer.size() - digitsCount - 1u] = '-';

	return negative ?
		std::string(
			bufPtr + sharedBuffer.size() - digitsCount - 1u,
			digitsCount + 1u
		)
		:
		std::string(
			bufPtr + sharedBuffer.size() - digitsCount,
			digitsCount
		);
}

template <typename T>
	requires
		std::same_as<T, std::int8_t> || std::same_as<T, std::uint8_t>
		||
		std::same_as<T, std::int16_t> || std::same_as<T, std::uint16_t>
		||
		std::same_as<T, std::int32_t> || std::same_as<T, std::uint32_t>
		||
		std::same_as<T, std::int64_t> || std::same_as<T, std::uint64_t>
std::optional<T> numFromStr(std::string_view str) noexcept {
	if (!(0u < str.size() && str.size() <= 20u))
		return std::nullopt;

	bool negative = str.starts_with('-');

	if (negative) {
		if (std::is_unsigned_v<T>)
			return std::nullopt;

		str = str.substr(1u);
	}

	alignas(32u) std::array<char, 32u> buffer;
	auto bufPtr = buffer.data();

	_mm256_store_si256(
		reinterpret_cast<__m256i*>(bufPtr),
		_mm256_set1_epi8('0')
	);
	std::memcpy(bufPtr, str.data(), str.size());

	auto ymmStr = _mm256_load_si256(reinterpret_cast<__m256i*>(bufPtr));
	auto ymmLower = _mm256_set1_epi8(47); // '0' - 1u
	auto ymmUpper = _mm256_set1_epi8(58); // '9' + 1u
	if (_mm256_movemask_epi8(
		_mm256_and_si256(
			_mm256_cmpgt_epi8(ymmStr, ymmLower),
			_mm256_cmpgt_epi8(ymmUpper, ymmStr)
		)
	) != -1) return std::nullopt;

	_mm256_store_si256(
		reinterpret_cast<__m256i*>(bufPtr),
		_mm256_sub_epi8(
			ymmStr,
			_mm256_set1_epi8('0')
		)
	);

	std::uint64_t factor = 1u;
	auto bufferRetView = buffer
							| std::views::take(str.size())
							| std::views::reverse;
	auto ret = std::ranges::fold_left(
		bufferRetView,
		0ull,
		[&factor](std::int64_t a, std::uint8_t i) {
			a += i * factor;
			factor *= 10u;

			return a;
		}
	);

	if (negative)
		ret = -ret;
	auto castedRet = static_cast<T>(ret);
	return castedRet == ret ? std::make_optional(castedRet) : std::nullopt;
}

} // namespace utils