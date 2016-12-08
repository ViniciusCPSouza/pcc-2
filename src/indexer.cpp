#include <indexer.h>


namespace indexer
{
	std::vector<std::chrono::nanoseconds> create_index(std::string filename, bool report)
	{
		std::string line;
		std::string idx_filename;
		std::chrono::high_resolution_clock::time_point report_start;
		std::chrono::high_resolution_clock::time_point report_end;
		std::vector<std::chrono::nanoseconds> runtimes;

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
			if (report) report_start = std::chrono::high_resolution_clock::now();
		  std::vector<int> sa = suffix_array::getSuffixArray(line);
		  if (report)
			{
				report_end = std::chrono::high_resolution_clock::now();
				runtimes.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(report_end - report_start));
			}

		  output.write_suffix_array(sa);
		  output.write_line(line);
		}

		input.close();
		output.close();

		return runtimes;
	}
}
