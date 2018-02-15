#include "../irc.h"

/*
** Action JOIN
*/
void	command_func_join(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository	*client;
	char			**channels;
	int				i;

	client = app->fds + client_fd;
	if (message->params_size == 0)
	{
		client_reply(client, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
		return ;
	}
	channels = ft_strsplit(message->params[0], ',');
	i = 0;
	while (channels[i])
	{
		if (!channel_add_client(app, channels[i], client))
		{
			channel_create(app, channels[i], NULL);
			channel_add_client(app, channels[i], client);
		}
		free(channels[i]);
		++i;
	}	
	free(channels);
}