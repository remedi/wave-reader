#include <Utils.h>

#include <cstdlib>

bool Utils::ParseFloat(const std::string& str, float& out) {
	char* end_ptr;
	float value = std::strtof(str.c_str(), &end_ptr);
	if (end_ptr == str.c_str() + str.size()) {
		out = value;
		return true;
	}
	return false;
}
