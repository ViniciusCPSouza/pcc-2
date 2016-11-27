#include <suffix_array.h>

// TODO: REMOVE THIIIIIIIIIIIIIIIIIS
#include <algorithm>


namespace suffix_array
{
	struct step_entry
	{
		int tuple[2];
		int original_index;
	};

	int cmp(struct step_entry a, struct step_entry b)
	{
	  return (a.tuple[0] == b.tuple[0]) ? (a.tuple[1] < b.tuple[1] ? 1: 0) : (a.tuple[0] < b.tuple[0] ? 1: 0);
	}

	std::vector<int> getSuffixArray(std::string text)
	{
		int N = text.length();
		std::vector<int> SA(N, 0);
		std::map<int, std::vector<int>> sort_index;
		step_entry tuple_array [N];

		// initialize the sort index
		sort_index[0] = std::vector<int>(N, 0);
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
			std::sort(tuple_array, tuple_array + N, cmp);

			sort_index[step] = std::vector<int>(N, 0);
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
