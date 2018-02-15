#include "../irc.h"

/*
** Get a channel from its name.
** 
** return the channel struct if found, otherwise return NULL.
*/
t_channel		*channel_get(t_app *app, char *name)
{
	t_list		*iter;
	t_channel	*channel_iter;

	iter = app->channels;
	while (iter)
	{
		channel_iter = (t_channel*)iter->content;
		if (ft_strcmp(channel_iter->name, name) == 0)
			return channel_iter;
		iter = iter->next;
	}
	return (NULL);
}