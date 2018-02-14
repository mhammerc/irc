#include "hash_table.h"

static void *memdup(void *mem, size_t size)
{
	char	*dup;

	dup = (char*)malloc(size);
	ft_memcpy(dup, mem, size);
	return (dup);
}

/*
** Insert a new element in the hash table ht.
**
** ht: hash table you previously created with ht_create
** key: key_size bytes wich are the unique identifier of el. key is duplicated.
** key_size: size of key in bytes
** el: element you want to store
*/
void		ht_insert(t_hash_table *ht, char *key, size_t key_size, void *el)
{
	unsigned int	index;
	t_hash_table_el	**el_storage;

	index = ht_hash(key, key_size);
	index = index % ht->size;
	el_storage = ht->elements + index;
	while (*el_storage != NULL)
	{
		el_storage = &((*el_storage)->next);
	}
	*el_storage = (t_hash_table_el*)malloc(sizeof(t_hash_table_el));
	(*el_storage)->next = NULL;
	(*el_storage)->key = memdup(key, key_size);
	(*el_storage)->key_size = key_size;
	(*el_storage)->el = el;
	++ht->elements_saved;
}