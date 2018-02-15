#include "../irc.h"

/*
** Action TOPIC.
** TODO: Topic editing rights.
** TODO: make function beautiful (delete redondant code)
*/
void	command_func_topic(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository		*client;
	t_channel			*channel;
	char				*reply;

	client = app->fds + client_fd;
	if (message->params_size == 0)
	{
		client_reply(client, ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");
		return ;
	}
	channel = channel_get(app, message->params[0]);
	if (!channel || !channel_have_client(app, message->params[0], client))
	{
		ft_sprintf(&reply, "%s :You're not on that channel", message->params[0]);
		client_reply(client, ERR_NOTONCHANNEL, reply);
		free(reply);
		return ;
	}
	if (message->params_size == 1)
	{
		if (!channel->description || ft_strlen(channel->description) == 0)
		{
			ft_sprintf(&reply, "%s :No topic is set", channel->name);
			client_reply(client, RPL_NOTOPIC, reply);
			free(reply);
			return ;
		}
		ft_sprintf(&reply, "%s :%s", channel->name, channel->description);
		client_reply(client, RPL_TOPIC, reply);
		free(reply);
		return ;
	}
	channel->description = ft_strdup(message->params[1]);
	if (ft_strlen(channel->description) == 0)
	{
		ft_sprintf(&reply, "%s :No topic is set", channel->name);
		client_reply(client, RPL_NOTOPIC, reply);
		free(reply);
		return ;
	}
	ft_sprintf(&reply, "%s :%s", channel->name, channel->description);
	client_reply(client, RPL_TOPIC, reply);
	free(reply);
}