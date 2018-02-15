#include "../irc.h"

void		channel_remove_client(t_app *app, char *channel_name, t_fd_repository *client)
{
	t_channel	*ch;
	t_list		**iter;

	ch = channel_get(app, channel_name);
	if (!ch)
	{
		fprintf(stderr, "Received invalid channel_name parameter on %s:%d",
		__FILE__, __LINE__);
		return ;
	}
	iter = &ch->clients;
	while (*iter)
	{
		if ((*iter)->content != client)
		{
			iter = &((*iter)->next);
			continue ;
		}
		*iter = (*iter)->next;
		--ch->num_clients;
		break ;
	}
}