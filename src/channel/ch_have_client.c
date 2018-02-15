#include "../irc.h"

/*
** Check if client is in the channel.
**
** Return 1 if yes, otherwise no.
*/
int		channel_have_client(t_app *app, char *channel_name, t_fd_repository *client)
{
	t_channel	*ch;
	t_list		*iter;

	ch = channel_get(app, channel_name);
	if (!ch)
	{
		fprintf(stderr, "Received invalid channel_name parameter on %s:%d",
		__FILE__, __LINE__);
		return (0);
	}
	iter = ch->clients;
	while (iter)
	{
		if (iter->content == client)
			return (1);
		iter = iter->next;
	}
	return (0);
}