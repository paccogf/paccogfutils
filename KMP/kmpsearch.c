#include <kmpsearch.h>
#include <plgtopmalloc.h>

int * kmp_compute_prefix_function(BYTE pattern[], int prefixtable[], int size)
{
	int k = -1;
	int i = 1;

	prefixtable[0] = k;
	for (i = 1; i < size; i++) 
	{
		while (k > -1 && pattern[k+1] != pattern[i])
		{
			k = prefixtable[k];
		}
		if (pattern[i] == pattern[k+1])
		{
			k++;
		}
		prefixtable[i] = k;
	}
return prefixtable;
}

int kmp_search_pattern(BYTE target[], int target_size, BYTE pattern[], int prefixtable[], int pattern_size)
{
	int i;
	int k = -1;

	for (i = 0; i < target_size; i++) 
	{
		while (k > -1 && pattern[k+1] != target[i])
		{
			k = prefixtable[k];
		}

		if (target[i] == pattern[k+1])
		{
			k++;
		}

		if (k == pattern_size - 1) 
		{
			return i-k;
		}
	}

	return -1;
}

/*
int kmp_search(BYTE target[], int target_size, BYTE pattern[], int pattern_size)
{
int *prefixtable = (int *) Pmalloc(pattern_size*sizeof(*prefixtable), 0);
int retval=-1;

	if(prefixtable != NULL)
	{
		kmp_compute_prefix_function(pattern, prefixtable, pattern_size);
		retval = kmp_search_pattern(target, target_size, pattern, prefixtable, pattern_size);

		Pfree(prefixtable);
	}

return retval;
}
*/

int KMP_Search(BYTE *target, int target_size, KMP_TERM *kmp_term)
{
	if(!kmp_term->initialized) // SI NO ESTA INICIALIZADA LA TABLA DE PREFIJOS, HACERLO AHORA
	{
		kmp_compute_prefix_function(kmp_term->search_term, kmp_term->prefix_table, kmp_term->length);
		kmp_term->initialized = TRUE;
	}

return kmp_search_pattern(target, target_size, kmp_term->search_term, kmp_term->prefix_table, kmp_term->length);
}





