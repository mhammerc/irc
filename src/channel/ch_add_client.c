#include "../irc.h"

int	channel_add_client(t_app *app, char *channel_name, t_fd_repository *client)
{
	t_channel	*ch;
	t_list		*new_el;

	if (channel_have_client(app, channel_name, client))
		return (1);
	ch = channel_get(app, channel_name);
	if (!ch)
	{
		return (0);
	}
	new_el = ft_lstnew(NULL, 0);
	new_el->content = client;
	new_el->content_size = sizeof(t_fd_repository);
	ft_lstadd(&ch->clients, new_el);
	++ch->num_clients;
	return (1);
}