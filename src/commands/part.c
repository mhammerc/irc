#include "../irc.h"

/*
** Action PART.
*/
void	command_func_part(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository	*client;
	char			**channels;
	t_channel		*channel;
	char			*reply;
	int				i;

	client = app->fds + client_fd;
	if (message->params_size == 0)
	{
		client_reply(client, ERR_NEEDMOREPARAMS, "PART :Not enough parameters");
		return ;
	}
	channels = ft_strsplit(message->params[0], ',');
	i = 0;
	while (channels[i])
	{
		channel = channel_get(app, channels[i]);
		if (!channel)
		{
			ft_sprintf(&reply, "%s :No such channel", channels[i]);
			client_reply(client, ERR_NOSUCHCHANNEL, reply);
			free(reply);
			free(channels[i]);
			++i;
			continue ;
		}
		if (!channel_have_client(app, channels[i], client))
		{
			ft_sprintf(&reply, "%s :You're not on that channel", channels[i]);
			client_reply(client, ERR_NOTONCHANNEL, reply);
			free(reply);
			free(channels[i]);
			++i;
			continue ;
		}
		channel_remove_client(app, channels[i], client);
		free(channels[i]);
		++i;
	}	
	free(channels);
}