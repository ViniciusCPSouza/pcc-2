#ifndef LZW_H
#define LZW_H

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


namespace lzw
{
	std::vector<int> compress_string(std::string input);
	std::string decompress_code(std::vector<int> input);
}

#endif