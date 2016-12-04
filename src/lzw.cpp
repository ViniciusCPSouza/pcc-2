#include <lzw.h>


namespace lzw
{
	// ###########################
	// ### CompressedInputFile ###
	// ###########################
	CompressedInputFile::CompressedInputFile(std::string filename)
	{
		this->stream = std::ifstream(filename, std::ios_base::binary);
		if (this->stream.fail()) throw std::runtime_error("LZW Decompression: Could not open input file!");
	}

	std::string CompressedInputFile::read_line()
	{
		std::vector<int> encoded;
		int code_count = 0;  // uint32_t

		this->stream.read((char*)&code_count, sizeof(code_count));

		int code = 0;
		for (int i = 0; i < code_count; i++)
		{
			this->stream.read((char*)&code, sizeof(code));
			encoded.push_back(code);
		}

		return this->decompress_code(encoded);
	}

	bool CompressedInputFile::has_lines()
	{
		return !this->stream.eof();
	}

	void CompressedInputFile::close()
	{
		this->stream.close();
	}

	std::string CompressedInputFile::decompress_code(std::vector<int> input)
	{
		std::ostringstream output;
		int N = input.size();
		
		// empty input -> empty decompressed string
		if (N == 0) return "";

		std::map<int, std::string> dict;
		char current_char = (char) input[0];
		std::string old_phrase = std::string(1, current_char);
		std::string phrase;
		output << current_char;
		int code = 256;
		int current_code;

		for (int c = 1; c < N; c++)
		{
			current_code = input[c];

			if (current_code < 256)
			{
				phrase = std::string(1, current_code);
			}
			else
			{
				// the code was already mapped
				if (dict.find(current_code) != dict.end())
				{
					phrase = dict[current_code];
				}
				else
				{
					phrase = old_phrase + current_char;
				}
			}

			output << phrase;
			current_char = phrase[0];
			dict[code] = old_phrase + current_char;
			code++;
			old_phrase = phrase;
		}

		return output.str();
	}

	// ###########################
	// ### CompressedOutupFile ###
	// ###########################

	CompressedOutputFile::CompressedOutputFile(std::string filename)
	{
		this->stream = std::ofstream(filename, std::ios_base::binary);
		if (this->stream.fail()) throw std::runtime_error("LZW Compression: Could not open output file!");
	}

	// TODO: too much space
	void CompressedOutputFile::write_line(std::string line)
	{
		std::vector<int> compressed = this->compress_string(line);
		int code_count = compressed.size();

		// writing the size of the list, so we can recover it later
		this->stream.write((char*)&code_count, sizeof(code_count));

		// writing the actual data
		for (std::vector<int>::iterator it = compressed.begin(); it != compressed.end(); it++)
		{
			this->stream.write((char*)&(*it), sizeof(*it));
		}
	}

	void CompressedOutputFile::close()
	{
		this->stream.flush();
		this->stream.close();
	}

	std::vector<int> CompressedOutputFile::compress_string(std::string input)
	{
		std::vector<int> output;
		int N = input.length();

		// empty string -> empty compression
		if (N == 0) return output;

		std::map<std::string, int> dict;
		char current_char;
		std::string phrase = std::string(input, 0, 1);;
		std::string phrase_sum;
		int code = 256;

		for (int c = 1; c < N; c++)
		{
			current_char = input[c];
			// the new phrase exists in the dict
			if (dict.find(phrase + current_char) != dict.end())
			{
				phrase += current_char;
			}
			// the new phrase still does not exist 
			else
			{
				output.push_back(phrase.length() > 1 ? dict[phrase] : phrase[0]);
				dict[phrase + current_char] = code;
				code++;
				phrase = std::string(1, current_char);
			}
		}

		output.push_back(phrase.length() > 1 ? dict[phrase] : phrase[0]);

		return output;
	}
}