#include <quicksort.h>


namespace quicksort
{
	void sort(utils::step_entry array[], int array_size)
	{
		__sort(array, 0, array_size - 1);
	}

	void __sort(utils::step_entry array[], int left, int right)
	{
		int i = left, j = right;
    utils::step_entry tmp;
    utils::step_entry pivot = array[(left + right) / 2];

    /* partition */
    while (i <= j)
    {
      while (lt(array[i], pivot)) i++;
      while (gt(array[j], pivot)) j--;
      if (i <= j)
      {
        tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
        i++;
        j--;
      }
    };

    if (left < j)
      __sort(array, left, j);
    if (i < right)
      __sort(array, i, right);
	}

	int lt(struct utils::step_entry a, struct utils::step_entry b)
	{
	  return (a.tuple[0] == b.tuple[0]) ? (a.tuple[1] < b.tuple[1] ? 1: 0) : (a.tuple[0] < b.tuple[0] ? 1: 0);
	}

	int gt(struct utils::step_entry a, struct utils::step_entry b)
	{
	  return (a.tuple[0] == b.tuple[0]) ? (a.tuple[1] > b.tuple[1] ? 1: 0) : (a.tuple[0] > b.tuple[0] ? 1: 0);
	}
}
