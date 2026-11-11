#pragma once

#include <string>

namespace Utils
{
	/// Parse floating point value from a string according to the current locale.
	/// @param str String to be parsed.
	/// @param out Reference to a floating point value.
	/// @return `true` if parsing succeeded, `false` otherwise.
	bool ParseFloat(const std::string& str, float& out);
}
