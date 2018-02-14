#include "hash_table.h"

/*
** Delete an hash table from the memory.
**
** ht: hash table you previously created with ht_create
**
** Please, call this function only if the hash table is empty.
** Else, it will have to loop over every elements to free them, it
** is very slow.
*/
void	ht_destroy(t_hash_table *ht)
{
	/*size_t			i;
	t_hash_table_el	*el_storage;
	t_hash_table_el	*el_storage_tmp;

	while (ht->elements_saved != 0 && i < ht->size)
	{
		if (ht->elements[i] == NULL)
		{
			++i;
			continue ;
		}
		el_storage = ht->elements[i];
		while (el_storage)
		{
			el_storage_tmp = el_storage;
			el_storage = el_storage->next;
			free(el_storage_tmp);
			--ht->elements_saved;
		}
		++i;
	}*/
	free(ht->elements);
	free(ht);
}