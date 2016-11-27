#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <vector>
#include <string>

namespace utils {
	struct step_entry
	{
		int tuple[2];
		int original_index;
	};
	
	std::vector<std::string> getPatterns(std::string filename);
}

#endif