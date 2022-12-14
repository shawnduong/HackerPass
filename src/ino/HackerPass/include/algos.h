#ifndef ALGOS_H
#define ALGOS_H

/* Binary search some ID array (uint32_t types) for some ID. Return the index
   if found, or -1 if not found. */
int32_t bsearch_id(uint32_t id, uint32_t *arr, uint16_t len)
{
	int32_t l = 0;
	int32_t r = len-1;
	int32_t m = 0;

	while (l <= r)
	{
		m = (l+r)/2;

		if (arr[m] < id)
			l = m + 1;
		else if (arr[m] > id)
			r = m - 1;
		else
			return m;
	}

	return -1;
}

/* Linearly search some ID array (uint32_t types) for some ID. This should be
   used instead of bsearch in cases where arr is not sorted. */
int32_t lsearch_id(uint32_t id, uint32_t *arr, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)  if (arr[i] == id)  return i;
	return -1;
}

#endif
