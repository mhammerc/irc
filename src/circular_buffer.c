#include "irc.h"

/*
** Init a buffer.
** You must destroy it with circular_buffer_destroy if you stop use it.
**
** size: size in bytes of the buffer
*/
void						circular_buffer_init(t_circular_buffer *b, size_t size)
{
	b->buffer = (char*)malloc(sizeof(char) * size);
	if (b->buffer == NULL)
		return ;
	b->buffer_size = size;
	b->begin = 0;
	b->end = 0;
}

/*
** Destroy a buffer.
** It must have been inited previously by circular_buffer_init().
*/
void						circular_buffer_destroy(t_circular_buffer *b)
{
	free(b->buffer);
	b->buffer = NULL;
	b->buffer_size = 0;
	b->begin = 0;
	b->end = 0;
}

/*
** Write new data to the buffer.
** It is similar as appending data.
** If input data is too large, only the end is kept due to the nature of a
** circular buffer.
**
** data: byte stream to append
** data_size: size of the byte stream
*/
/*

   |________|
    ---------
 abs(10-12) = abs(-2) = 2
 abs(10-9) = abs(1) = 1
*/
void						circular_buffer_write(t_circular_buffer *b, char *data, size_t data_size)
{
	size_t		quantity_to_copy;
	size_t		remaining_to_copy;
	size_t		offset;

	offset = 0;
	if (data_size > b->buffer_size)
		offset = data_size - b->buffer_size;
	data += offset;
	data_size -= offset;
	quantity_to_copy = MIN(data_size, b->buffer_size - b->end);
	remaining_to_copy = data_size - quantity_to_copy;
	ft_memcpy(b->buffer + b->end, data, quantity_to_copy);
	b->end += quantity_to_copy;
	if (remaining_to_copy == 0)
		return;
	b->end = remaining_to_copy;
	ft_memcpy(b->buffer, data + quantity_to_copy, remaining_to_copy);
	if (b->end > b->begin)
		b->begin = b->end;
}

/*
** Read previously written bytes.
** For ease of use, it return the size of the result which will be malloced. You have to free it.
**
** It search for the byte 'delimiter' in the buffer.
** If it find the delimiter, it return up to the delimiter (including)
** and invalidate read data.
**
** Data read from this function is lost forever from the circular buffer.
**
** For your convenience, a \0 is added on the end of the returned byte stream.
** The returned size doesn't count the \0.
**
** If the byte stream isn't found, return 0.
*/
size_t						circular_buffer_read(t_circular_buffer *b, char delimiter, char **result)
{
	size_t		pos;
	int			found;
	int			from_0;

	pos = b->begin;
	found = 0;
	from_0 = 0;
	while (pos < b->buffer_size)
	{
		if (b->buffer[pos] == delimiter)
		{
			found = 1;
			break;
		}
		if (pos == b->end)
			break;
		++pos;
	}
	/* delimiter not found, start from zero */
	if (!found && pos != b->end)
	{
		from_0 = 1;
		pos = 0;
		while (pos <= b->end)
		{
			if (b->buffer[pos] == delimiter)
			{
				found = 1;
				break;
			}
			++pos;
		}
	}
	if (!found)
		return (0);
	
	size_t		result_size;

	result_size = pos - b->begin + 1;
	if (from_0)
		result_size = (b->buffer_size - b->begin) + pos;
	*result = (char*)malloc(result_size + 1);	
	ft_memcpy((*result), b->buffer + b->begin, MIN(result_size, b->buffer_size - b->begin));
	if (from_0)
		ft_memcpy((*result) + b->buffer_size - b->begin, b->buffer, pos);
	b->begin = pos + 1;
	(*result)[result_size] = '\0';
	return (result_size);
}