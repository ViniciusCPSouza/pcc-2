#ifndef INDEXER_H
#define INDEXER_H

#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <lzw.h>
#include <suffix_array.h>

namespace indexer
{
	std::vector<std::chrono::nanoseconds> create_index(std::string filename, bool report);
}

#endif