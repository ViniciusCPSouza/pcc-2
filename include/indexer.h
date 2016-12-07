#ifndef INDEXER_H
#define INDEXER_H

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <lzw.h>
#include <suffix_array.h>

namespace indexer
{
	void create_index(std::string filename);
}

#endif