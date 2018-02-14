#include "hash_table.h"

/*
** Search an element in the hash table.
**
** ht: hash table you previously created with ht_create
** key: key_size bytes wich are the unique identifier of el.
** key_size: size of key in bytes
**
** return the element you previously saved using the key and key_size.
*/
void	*ht_search(t_hash_table *ht, char *key, size_t key_size)
{
	unsigned long	index;
	t_hash_table_el	*el_storage;

	index = ht_hash(key, key_size);
	index = index % ht->size;
	el_storage = ht->elements[index];
	while (el_storage)
	{
		if (el_storage->key_size == key_size
			&& memcmp(el_storage->key, key, key_size) == 0)
			return (el_storage->el);
		el_storage = el_storage->next;
	}
	return (NULL);
}