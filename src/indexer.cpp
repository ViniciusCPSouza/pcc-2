#include <indexer.h>

// TODO: REMOOOOOOOOOOOVE
#include <iostream>


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
		std::ofstream output(idx_filename);

		if(input.fail())
		{
	    throw std::runtime_error("Can't index non existing input!");
	  }
	  if(output.fail())
		{
	    throw std::runtime_error("Can't write to idx file!");
	  }

		while (std::getline(input, line))
		{
		  std::vector<int> sa = suffix_array::getSuffixArray(line);
		  output << get_sa_string(sa) << std::endl;
		  output << line << std::endl;

		  std::cout << "==========================" << std::endl;
		  std::vector<int> lzw_sa = lzw::compress_string(get_sa_string(sa));
		  std::vector<int> lzw_line = lzw::compress_string(line);

		  for(std::vector<int>::iterator it = lzw_sa.begin(); it!= lzw_sa.end(); it++)
		  {
		  	std::cout << *it << " ";
		  }
		  std::cout << std::endl;
		  std::cout << lzw::decompress_code(lzw_sa) << std::endl;

		  std::cout << "---" << std::endl;

		  for(std::vector<int>::iterator it = lzw_line.begin(); it!= lzw_line.end(); it++)
		  {
		  	std::cout << *it << " ";
		  }
		  std::cout << std::endl;
		  std::cout << lzw::decompress_code(lzw_line) << std::endl;
		  std::cout << "==========================" << std::endl;
		}

		input.close();
		output.close();
	}

	std::string get_sa_string(std::vector<int> sa)
	{
		std::ostringstream line;

		for (std::vector<int>::iterator it = sa.begin(); it != sa.end(); it++)
		{
			if (it != sa.begin()) line << ":";
			line << *it;
		}

		return line.str();
	}
}
