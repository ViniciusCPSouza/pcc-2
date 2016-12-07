#ifndef LZW_H
#define LZW_H

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
			std::string decompress_code(std::vector<int> input);

		public:
			CompressedInputFile(std::string filename);
			std::string read_line();
			bool has_lines();
			void close();
	};

	class CompressedOutputFile
	{
		private:
			std::ofstream stream;
			std::vector<int> compress_string(std::string input);

		public:
			CompressedOutputFile(std::string filename);
			void write_line(std::string line);
			void close();
	};
}

#endif