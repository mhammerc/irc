#include "../irc.h"

/*
** Remove a channel from the channel list.
*/
void		channel_remove(t_app *app, char *name)
{
	t_list		**iter;
	t_list		*to_delete;
	t_channel	*channel_iter;

	iter = &app->channels;
	to_delete = NULL;
	while (*iter)
	{
		channel_iter = (t_channel*)(*iter)->content;
		if (ft_strcmp(channel_iter->name, name) == 0)
		{
			to_delete = *iter;
			*iter = (*iter)->next;
			channel_free(app, (t_channel**)(&to_delete->content));	
			free(to_delete);
		}
		iter = &((*iter)->next);
	}
}