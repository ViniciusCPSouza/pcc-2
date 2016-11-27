#include <searcher.h>


namespace searcher
{
	enum LineType {SUFFIX_ARRAY, TEXT_LINE};

	std::map<int, std::vector<int>> search_index(std::string index_file, std::vector<std::string> patterns)
	{
		std::map<int, std::vector<int>> occurrences;

		std::ifstream input(index_file);
		std::string line;

		LineType lineType = SUFFIX_ARRAY;
		std::vector<int> sa;
		int current_line = -1;
		while (std::getline(input, line))
		{
			if (lineType == SUFFIX_ARRAY)
			{
				sa = get_suffix_array(line);
				lineType = TEXT_LINE;
			}
			else
			{
				current_line++;
				lineType = SUFFIX_ARRAY;

				// search in line for the occurrences
				occurrences[current_line] = search(line, sa);
			}
		}

		input.close();

		return occurrences;
	}

	std::vector<int> get_suffix_array(std::string file_line)
	{
		std::vector<int> sa;
		std::istringstream ss(file_line);
		std::string token;

		while(std::getline(ss, token, ':')) sa.push_back(atoi(token.c_str()));

		return sa;
	}

	std::vector<int> search(std::string text, std::vector<int> sa)
	{
		std::vector<int> occurrences;

		// TODO: search
		// ...

		return occurrences;
	}
}
