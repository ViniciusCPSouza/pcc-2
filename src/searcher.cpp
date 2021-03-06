#include <searcher.h>


namespace searcher
{
	enum LineType {SUFFIX_ARRAY, TEXT_LINE};

	std::map<int, LineResult> search_index(std::string index_file, std::vector<std::string> patterns, bool report)
	{
		std::map<int, LineResult> occurrences;
		std::chrono::high_resolution_clock::time_point report_start;
		std::chrono::high_resolution_clock::time_point report_end;
		lzw::CompressedInputFile input(index_file);
		std::string line;

		LineType lineType = SUFFIX_ARRAY;
		std::vector<int> sa;
		int current_line = -1;
		while (input.has_lines())
		{
			if (lineType == SUFFIX_ARRAY)
			{
				sa = input.read_suffix_array();
				lineType = TEXT_LINE;
			}
			else
			{
				line = input.read_line();
				current_line++;
				lineType = SUFFIX_ARRAY;

				occurrences[current_line] = LineResult();
				occurrences[current_line].occurrences = std::vector<int>();
				std::vector<int> results;

				if (report) report_start = std::chrono::high_resolution_clock::now();
				for (std::vector<std::string>::iterator it = patterns.begin(); it != patterns.end(); it++)
				{
					results = search(line, sa, *it);
					occurrences[current_line].occurrences.insert(occurrences[current_line].occurrences.end(), results.begin(), results.end());
				}
				if (report)
				{
					report_end = std::chrono::high_resolution_clock::now();
					occurrences[current_line].runtime = std::chrono::duration_cast<std::chrono::nanoseconds>(report_end - report_start);
				}

				if (occurrences[current_line].occurrences.size() > 0) occurrences[current_line].line = line;
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

	std::vector<int> search(std::string text, std::vector<int> sa, std::string pattern)
	{
		std::vector<int> occurrences;
		int N = pattern.length();
		int T = text.length();

		if (N == 0 || T == 0 || (compare(pattern, text, sa[0]) < 0) || (compare(pattern, text, sa[T-1]) > 1)) return occurrences;

		int L = 0, R = T - 1, M = 0, comp;
		bool found = false;
		while(R - L > 1)
		{
			M = (R + L) / 2;
			comp = compare(pattern, text, sa[M]);

			if (comp < 0)
			{
				R = M;
			}
			else if (comp > 0)
			{
				L = M;
			}
			else
			{
				L = R = M;
				found = true;
			}
		}

		// occ!
		if (found)
		{
			// find the neighbours
			occurrences.push_back(sa[M]);

			for (int nl = M - 1, nr = M + 1; nl >= 0 && nr < T; nl--, nr++)
			{
				// occ!
				if (compare(pattern, text, sa[nl]) == 0)
				{
					occurrences.insert(occurrences.begin(), sa[nl]);
				}
				else
				{
					nl = -1;
				}

				// occ!
				if (compare(pattern, text, sa[nr]) == 0)
				{
					occurrences.push_back(sa[nr]);
				}
				else
				{
					nr = T;	
				}
			}
		}

		return occurrences;
	}

	int compare(std::string pattern, std::string text, int offset)
	{
		// if the patter is bigger than the prefix, we can only be lesser than
		// or grater than, but never be included in the prefix,
		// otherwise, we start with the supposition that we are included
		// in the suffix
		int status = (pattern.length() <= (text.length() - offset)) ? 0 : 1;

		for(int p = 0, t = offset; p < pattern.length() && t < text.length(); p++, t++)
		{
			if (pattern[p] < text[t])
			{
				status = -1;
				break;
			}

			if (pattern[p] > text[t])
			{
				status = 1;
				break;	
			}
		}

		return status;
	}
}
