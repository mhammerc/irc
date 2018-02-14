#ifndef HASH_TABLE_H
# define HASH_TABLE_H

# include <stddef.h>
# include <stdio.h>

# include "../../libft/libft.h"

/*
** Hash table mini-library
*/

typedef struct				s_hash_table_el
{
	struct s_hash_table_el	*next;
	char					*key;
	size_t					key_size;
	void					*el;
}							t_hash_table_el;

typedef struct				s_hash_table
{
	t_hash_table_el			**elements;
	size_t					size;
	size_t					elements_saved;
}							t_hash_table;

t_hash_table				*ht_create(size_t size);
void						ht_insert(t_hash_table *ht, char *key, size_t key_size, void *el);
void						*ht_search(t_hash_table *ht, char *key, size_t key_size);
void						*ht_delete(t_hash_table *ht, char *key, size_t key_size);
void						ht_destroy(t_hash_table *ht);
size_t						ht_num_elements_saved(t_hash_table *ht);

unsigned int				ht_hash(char *key, size_t key_size);

#endif