#ifndef SEARCHER_H
#define SEARCHER_H

#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>


namespace searcher
{
	std::map<int, std::vector<int>> search_index(std::string index_file, std::vector<std::string> patterns);
	std::vector<int> get_suffix_array(std::string file_line);
	std::vector<int> search(std::string text, std::vector<int> sa);
}

#endif