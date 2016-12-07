#include <indexer.h>


namespace indexer
{
	void create_index(std::string filename)
	{
		std::string line;
		std::string idx_filename;

		// getting idx file name
		idx_filename = filename.substr(0, filename.find_last_of("."));
	  idx_filename = idx_filename.substr(idx_filename.find_last_of("/") + 1);
	  idx_filename = idx_filename + ".idx";

		std::ifstream input(filename);
		lzw::CompressedOutputFile output(idx_filename);

		if(input.fail())
		{
	    throw std::runtime_error("Can't index non existing input!");
	  }

		while (std::getline(input, line))
		{
		  std::vector<int> sa = suffix_array::getSuffixArray(line);
		  output.write_suffix_array(sa);
		  output.write_line(line);
		}

		input.close();
		output.close();
	}
}
