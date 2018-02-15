#include "../irc.h"

void	channel_add_client(t_app *app, char *channel_name, t_fd_repository *client)
{
	t_channel	*ch;
	t_list		*new_el;

	ch = channel_get(app, channel_name);
	if (!ch)
	{
		fprintf(stderr, "Received invalid channel_name parameter on %s:%d",
		__FILE__, __LINE__);
		return ;
	}
	new_el = ft_lstnew(NULL, 0);
	new_el->content = client;
	new_el->content_size = sizeof(t_fd_repository);
	ft_lstadd(&ch->clients, new_el);
	++ch->num_clients;
}