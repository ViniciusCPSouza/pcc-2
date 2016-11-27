#ifndef INDEXER_H
#define INDEXER_H

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <suffix_array.h>

namespace indexer
{
	void create_index(std::string filename);
	std::string get_sa_string(std::vector<int> sa);
}

#endif