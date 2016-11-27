#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <utils.h>

namespace quicksort
{
	void sort(utils::step_entry array[], int array_size);
	void __sort(utils::step_entry array[], int left, int right);
	int lt(struct utils::step_entry a, struct utils::step_entry b);
	int gt(struct utils::step_entry a, struct utils::step_entry b);
}

#endif