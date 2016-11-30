#include <lzw.h>


namespace lzw
{
	// ###########################
	// ### CompressedInputFile ###
	// ###########################
	CompressedInputFile::CompressedInputFile(std::string filename)
	{
		// TODO
	}

	std::string CompressedInputFile::read_line()
	{
		// TODO
		return "";
	}

	void CompressedInputFile::close()
	{
		// TODO
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
		// TODO
	}

	void CompressedOutputFile::write_line(std::string)
	{
		// TODO
	}

	void CompressedOutputFile::close()
	{
		// TOOD
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