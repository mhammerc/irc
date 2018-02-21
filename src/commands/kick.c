#include "../irc.h"

/*
** Count how many time 'c' appear in 'str'.
*/
static size_t	strchrcount(char *str, char c)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str == c)
			++count;
		++str;
	}
	return (count);
}

/*
** Action KICK.
** TODO: Replies (with comment).
*/
void			command_func_kick(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository	*client;
	size_t			count_channels;
	size_t			count_users;
	char			**channels;
	char			**users;
	size_t			i;

	client = app->fds + client_fd;
	if (message->params_size < 2)
	{
		client_reply(client, ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");
		return ;
	}
	if (!(client->client_info.mode & CLIENT_MODE_OPERATOR))
	{
		client_reply(client, ERR_CHANOPRIVSNEEDED, ":You're not operator");
		return ;
	}
	count_channels = strchrcount(message->params[0], ',');
	count_users = strchrcount(message->params[1], ',');
	if (count_channels != 0 && !(count_channels == count_users))
	{
		client_reply(client, ERR_BADCHANMASK, ":Bad Channel Mask");
		return ;
	}
	channels = ft_strsplit(message->params[0], ',');
	users = ft_strsplit(message->params[1], ',');
	if (count_channels == 0)
	{
		i = 0;
		while (users[i])
		{
			t_fd_repository	*invited = client_get_by_nick(app, users[i]);
			if (invited && channel_have_client(app, channels[0], invited))
			{
				channel_remove_client(app, channels[0], invited);
				// send rpl reply
			}
			else
			{
				// send err reply
			}
			++i;
		}
	}
	else
	{
		i = 0;
		while (users[i])
		{
			t_fd_repository	*invited = client_get_by_nick(app, users[i]);
			if (invited && channel_get(app, channels[i]) && channel_have_client(app, channels[i], invited))
			{
				channel_remove_client(app, channels[i], invited);
			}
			else
			{
				// send err reply
			}
			++i;
		}
	}
}