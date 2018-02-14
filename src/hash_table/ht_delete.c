#include "hash_table.h"

/*
** Search and delete an element in the hash table.
**
** ht: hash table you previously created with ht_create
** key: key_size bytes wich are the unique identifier of el.
** key_size: size of key in bytes
**
** return the previously inserted element if it was successfully deleted.
** return 0 if the element wasn't found, and not deleted.
*/
void	*ht_delete(t_hash_table *ht, char *key, size_t key_size)
{
	unsigned long	index;
	t_hash_table_el	**el_storage;
	t_hash_table_el *previous;
	void			*el;

	index = ht_hash(key, key_size);
	index = index % ht->size;
	el_storage = ht->elements + index;
	previous = NULL;
	while (*el_storage)
	{
		if ((*el_storage)->key_size == key_size
			&& memcmp((*el_storage)->key, key, key_size) == 0)
			break;
		previous = *el_storage;
		el_storage = &((*el_storage)->next);
	}
	if (el_storage == NULL)
		return (0);
	free((*el_storage)->key);
	if (previous)
		previous->next = (*el_storage)->next;
	el = (*el_storage)->el;
	free(*el_storage);
	--ht->elements_saved;
	return (el);
}