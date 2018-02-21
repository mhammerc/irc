#include "../irc.h"

/*
** Action INVITE.
** TODO: send replies.
*/
void	command_func_invite(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository	*client;
	t_fd_repository	*invited_client;
	t_channel		*channel;
	char			*reply;
	int				is_channel_created;

	client = app->fds + client_fd;
	reply = NULL;
	if (message->params_size < 2)
	{
		client_reply(client, ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters");
		return ;
	}
	invited_client = client_get_by_nick(app, message->params[0]);
	if (!invited_client)
	{
		ft_sprintf(&reply, "%s :No such nick/channel", message->params[0]);
		client_reply(client, ERR_NOSUCHNICK, reply);
		free(reply);
		return ;
	}
	channel = channel_get(app, message->params[1]);
	is_channel_created = 0;
	if (!channel)
	{
		is_channel_created = 1;
		channel_create(app, message->params[1], NULL);
		channel = channel_get(app, message->params[1]);
	}
	if (!is_channel_created && !channel_have_client(app, message->params[1], client))
	{
		ft_sprintf(&reply, "%s :You're not on that channel", message->params[1]);
		client_reply(client, ERR_NOTONCHANNEL, reply);
		free(reply);
		return ;
	}
	if (channel_have_client(app, message->params[1], invited_client))
	{
		ft_sprintf(&reply, "%s %s :is already on channel", message->params[0], message->params[1]);
		client_reply(client, ERR_USERONCHANNEL, reply);
		free(reply);
		return ;
	}
	if (!is_channel_created && channel->mode & CHANNEL_MODE_INVITE_ONLY && !(client->client_info.mode & CLIENT_MODE_OPERATOR))
	{
		ft_sprintf(&reply, "%s :You're not channel operator", message->params[1]);
		client_reply(client, ERR_CHANOPRIVSNEEDED, reply);
		free(reply);
		return ;
	}
	channel_add_client(app, message->params[1], invited_client);
}