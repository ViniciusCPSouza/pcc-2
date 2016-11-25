#include <suffix_array.h>

// TODO: REMOVE THIIIIIIIIIIIIIIIIIS
#include <iostream>

#define MAX_CHAR 255

namespace suffix_array {
	std::vector<int> radixSort(std::string original, std::vector<int> sa, int max_value, int offset) {
		std::vector<int> output(sa.size(), 0);
		int count[max_value + 1];

		// set all counters to 0
		for (int i = 0; i <= max_value; i++) count[i] = 0;
		// count each occurrence in the origin string, using the current offset
		for (std::vector<int>::iterator it = sa.begin(); it != sa.end(); it++)
		{
			count[original[(*it) + offset]]++;
		}
		// exclusive prefix sum
		for (int i = 0, sum = 0; i <= max_value; i++){
			int old_count = count[i];
			count[i] = sum;
			sum += old_count;
		}
		// actually sort
		for (std::vector<int>::iterator it = sa.begin(); it != sa.end(); it++)
		{
			output[count[original[*it + offset]]++] = *it;
		}

		return output;
	}

	std::vector<int> getSuffixArray(std::string text)
	{
		std::vector<int> sa;

		if (text.length() == 0) return sa;

		// [1] Construct the suffix array A12 of the suffixes starting at positions i diff 0 (mod 3).
		// This is done by a recursive call of the skew algorithm for a string of two thirds the length.

		//// [1.1] Lexicographically name all triples T[i..i + 2]
		text = text.append(3, (char)0);

		std::vector<int> lex_names;
		for (int i = 1; i < text.length() - 3; i++)
		{
			if ((i % 3) != 0)
			{
				lex_names.push_back(i);
			}
		}

		// three passes of radix sort, for each character of the triple
		lex_names = radixSort(text, lex_names, MAX_CHAR, 2);
		lex_names = radixSort(text, lex_names, MAX_CHAR, 1);
		lex_names = radixSort(text, lex_names, MAX_CHAR, 0);

		for (std::vector<int>::iterator it = lex_names.begin(); it != lex_names.end(); it++)
		{
			std::cout << *it << "," << std::endl;
		}

		//// [1.2] Construct a text T' of triple names

		//// [1.3] Construct suffix array A' of T' (recursively)

		//// [1.4] Transform A' into A12

		// [2] Construct the suffix array A0 of the remaining suffixes using the result of the first step.
		// ...
		
		// [3] Merge the two suffix arrays into one.
		// ...

		return sa;
	}
}
