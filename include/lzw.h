#ifndef LZW_H
#define LZW_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>


namespace lzw
{
	class CompressedInputFile
	{
		private:
			std::ifstream stream;
			std::string decompress_code(std::vector<uint16_t> input);

		public:
			CompressedInputFile(std::string filename);
			std::string read_line();
			std::vector<int> read_suffix_array();
			bool has_lines();
			void close();
	};

	class CompressedOutputFile
	{
		private:
			std::ofstream stream;
			std::vector<uint16_t> compress_string(std::string input);

		public:
			CompressedOutputFile(std::string filename);
			void write_line(std::string line);
			void write_suffix_array(std::vector<int> sa);
			void close();
	};
}

#endif