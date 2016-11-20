#include "utils.h"

namespace utils {
	std::vector<std::string> getPatterns(std::string filename)
	{
		std::vector<std::string> patterns;
		std::ifstream file(filename);
		std::string str;

		if(file.fail()){
      throw std::runtime_error("Patterns file does not exist!");
    }

		while (std::getline(file, str))
		{
		  patterns.push_back(str);
		}

		file.close();

		return patterns;
	}
}
