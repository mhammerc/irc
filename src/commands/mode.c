#include "../irc.h"

/*
** The user requested or set its modes.
**
** Response string is 8 bytes long max (counting ending nul-char):
** +[aiwroO]\0
*/
static void		reply_user_modes(t_fd_repository *client)
{
	char	modes_str[8];
	int		i;

	ft_bzero(modes_str, 8);
	modes_str[0] = '+';
	i = 1;
	if (client->client_info.mode & CLIENT_MODE_AWAY)
		modes_str[i++] = 'a';
	if (client->client_info.mode & CLIENT_MODE_INVISIBLE)
		modes_str[i++] = 'i';
	if (client->client_info.mode & CLIENT_MODE_WALLOPS)
		modes_str[i++] = 'w';
	if (client->client_info.mode & CLIENT_MODE_RESTRICTED)
		modes_str[i++] = 'r';
	if (client->client_info.mode & CLIENT_MODE_OPERATOR)
		modes_str[i++] = 'o';
	if (client->client_info.mode & CLIENT_MODE_LOCAL_OPERATOR)
		modes_str[i++] = 'O';
	client_reply(client, RPL_UMODEIS, modes_str);	
}

static void		set_user_modes(t_fd_repository *client, t_irc_message *message)
{
	size_t	i;
	int 	j;
	char	*param;
	int 	operation;
	int		new_mode;

	i = 1;
	new_mode = client->client_info.mode;
	while (i < message->params_size)
	{
		j = 0;
		param = message->params[i];
		operation = 0;
		while (param[j])
		{
			if (param[j] == '+')
				operation = 1;
			else if (param[j] == '-')
				operation = -1;
			else if (param[j] == 'i')
			{
				if (operation == 1)
					new_mode |= CLIENT_MODE_INVISIBLE;
				if (operation == -1)
					new_mode &= ~CLIENT_MODE_INVISIBLE;
			}
			else if (param[j] == 'w')
			{
				if (operation == 1)
					new_mode |= CLIENT_MODE_WALLOPS;
				if (operation == -1)
					new_mode &= ~CLIENT_MODE_WALLOPS;
			}
			else if (param[j] == 'r')
			{
				if (operation == 1)
					new_mode |= CLIENT_MODE_RESTRICTED;
			}
			else if (param[j] == 'o')
			{
				if (operation == -1)
					new_mode &= ~CLIENT_MODE_OPERATOR;
			}
			else if (param[j] == 'O')
			{
				if (operation == -1)
					new_mode &= ~CLIENT_MODE_LOCAL_OPERATOR;
			}
			else
			{
				client_reply(client, ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag");
				return ;
			}
			++j;
		}
		++i;
	}
	client->client_info.mode = new_mode;
	reply_user_modes(client);
}

/*
** Action MODE
*/
void	command_func_mode(t_app *app, int _client_fd, t_irc_message *message)
{
	t_fd_repository	*client_fd;

	client_fd = app->fds + _client_fd;
	if (message->params_size == 0)
	{
		client_reply(client_fd, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
		return ;
	}
	if (ft_strcmp(message->params[0], client_fd->client_info.nick) != 0)
	{
		client_reply(client_fd, ERR_USERSDONTMATCH, ":Cannot change mode for other users");
		return ;
	}
	if (message->params_size == 1)
	{
		reply_user_modes(client_fd);
		return ;
	}
	else
	{
		set_user_modes(client_fd, message);
		return ;
	}
}