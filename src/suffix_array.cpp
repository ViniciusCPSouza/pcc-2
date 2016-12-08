#include <suffix_array.h>
#include <utils.h>
#include <quicksort.h>
#include <iostream>

#define MAXN 65536
#define MAXLG 17

namespace suffix_array
{
	std::vector<int> getSuffixArray(std::string text)
	{
		int N = text.length();
		std::vector<int> SA(N, 0);
		int sort_index [MAXLG][MAXN];
		utils::step_entry tuple_array [N];

		// initialize the sort index
		for (int i = 0; i < N; i++)
		{
			sort_index[0][i] = text[i] - 'a';
		}

		int step, count;
		for(step=1, count=1; count < N; step++, count *= 2)
		{
			// current iteration of the tuple list
			for (int i = 0; i < N; i++)
			{
        tuple_array[i].tuple[0] = sort_index[step - 1][i];
        tuple_array[i].tuple[1] = (i + count) < N ? sort_index[step - 1][i + count] : -1;
        tuple_array[i].original_index = i;
			}

			// sort the new tuples
			quicksort::sort(tuple_array, N);

			for (int i = 0; i < N; i++)
			{
				if ((i > 0) && (tuple_array[i].tuple[0] == tuple_array[i - 1].tuple[0]) &&
					  (tuple_array[i].tuple[1] == tuple_array[i - 1].tuple[1]))
				{
					sort_index[step][tuple_array[i].original_index] = sort_index[step][tuple_array[i - 1].original_index];
				}
				else
				{
					sort_index[step][tuple_array[i].original_index] = i;
				}
			}
		}

		for (int i = 0; i < N; i++)
		{
			SA[i] = tuple_array[i].original_index;
		}

		return SA;
	}
}
