#include "utility_funcs.h"

std::string vec_to_str(std::vector<std::string> vec) {
	std::string s;

	for (int i = 0; i < vec.size(); i++) {
		s += vec[i] + "\n";
	}

	return s;
}