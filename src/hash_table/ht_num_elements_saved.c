#include "hash_table.h"

/*
** Get the number of elements saved in the hash table.
**
** ht: hash table you previously created with ht_create
*/
size_t			ht_num_elements_saved(t_hash_table *ht)
{
	return (ht->elements_saved);
}