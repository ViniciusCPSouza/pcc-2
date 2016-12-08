#ifndef SEARCHER_H
#define SEARCHER_H

#include <chrono>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include <lzw.h>


namespace searcher
{
	struct LineResult
	{
		std::string line;
		std::vector<int> occurrences;
		std::chrono::nanoseconds runtime;
	};

	std::map<int, LineResult> search_index(std::string index_file, std::vector<std::string> patterns, bool report);
	std::vector<int> get_suffix_array(std::string file_line);
	std::vector<int> search(std::string text, std::vector<int> sa, std::string pattern);
	int compare(std::string pattern, std::string text, int offset);
}

#endif