#include <lzw.h>


namespace lzw
{
	std::vector<int> compress_string(std::string input)
	{
		std::map<std::string, int> dict;
		std::vector<int> output;
		int N = input.length();
		int next_code = 256;

		if (N == 0) return output;

		// init the dictionary with the ASCII chars
		for (int i = 0; i < 256; i++) dict[std::string(1, i)] = i;

		// iterate over the string characters
		std::string phrase = std::string(input, 0, 1);
		char current_char;
		for (int c = 1; c < N; c++)
		{
			current_char = input[c];

			// the new phrase already has a proper code
			if (dict.find(phrase + current_char) != dict.end())
			{
				phrase += current_char;
			}
			else
			{
				output.push_back(dict[phrase]);
				dict[phrase + current_char] = next_code;
				next_code++;
				phrase = std::string(1, current_char);
			}
		}

		// push the last code
		output.push_back(dict[phrase]);

		return output;
	}

	std::string decompress_code(std::vector<int> input)
	{
		std::map<int, std::string> dict;
		std::ostringstream output;
		int N = input.size();
		int next_code = 256;

		if (N == 0) return "";

		int current_char = input[0];
		output << (char) current_char;

		std::string old_phrase = std::string(1, current_char);
		std::string phrase;
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
				if (dict.find('_' + current_code) != dict.end())
				{
					phrase = dict['_' + current_code];
				}
				else
				{
					phrase = old_phrase + std::string(1, current_char);
				}

				output << phrase;
				current_char = phrase[0];
				dict['_' + next_code] = old_phrase + std::string(1, current_char);
        next_code++;
        old_phrase = phrase;
			}
		}
		
		return output.str();
	}
}