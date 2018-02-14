#include "hash_table.h"

/*
** Create and return a new hash table.
**
** size: Size of the hash table. Adapt it according to your dataset size.
**       The hash table will not be limited to size elements but the more
**       it approach to size elements, the slower it will be.
**
** return the hash table pointer, you will have to pass it as parameter on next
** functions.
*/
t_hash_table	*ht_create(size_t size)
{
	t_hash_table	*ht;

	ht = (t_hash_table*)malloc(sizeof(t_hash_table));
	if (!ht)
		return (NULL);
	ht->size = size;
	ht->elements = (t_hash_table_el**)malloc(sizeof(t_hash_table_el*) * size);
	ht->elements_saved = 0;
	if (!ht->elements)
	{
		free(ht);
		return (NULL);
	}
	ft_bzero(ht->elements, sizeof(t_hash_table_el*) * size);
	return (ht);
}