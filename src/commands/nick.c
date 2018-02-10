#include "../irc.h"

/*
** From RFC 2812
** special = %x5B-60 / %x7B-7D
**   ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
*/
static int	is_special(char byte)
{
	if (byte >= 0x5B && byte <= 0x60)
		return (1);
	return (0);
}

/*
** From RFC 2812
** nickname = ( letter / special ) *8( letter / digit / special / "-" )
*/
static int	check_nickname(char *nickname)
{
	if (!ft_isalpha(nickname[0]) && !(is_special(nickname[0])))
	{
		return (0);
	}
	if (ft_strlen(nickname) > 9)
		return (0);
	++nickname;
	while (*nickname)
	{
		if (!ft_isalnum(*nickname) && !is_special(*nickname) && *nickname != '-')
			return (0);
		++nickname;
	}
	return (1);
}	

/*
** Action to NICK command.
**
** TODO: ERR_NICKNAMEINUSE
** TODO: ERR_UNAVAILRESOURCE
*/
void		command_func_nick(t_app *app, int _client_fd, t_irc_message *message)
{
	t_fd_repository		*client_fd;
	char				*reply;

	client_fd = app->fds + client_fd;
	reply = NULL;
	if (message->params_size < 1)
	{
		ft_sprintf(&reply, ":127.0.0.1 %s %s", ERR_NONICKNAMEGIVEN, ":No nickname given");
		client_reply(client_fd, reply);
		free(reply);
		return ;
	}
	if (!check_nickname(message->params[0]))
	{
		ft_sprintf(&reply, ":127.0.0.1 %s %s %s", ERR_ERRONEUSNICKNAME, message->params[0], ":Nickname is already in use");
		client_reply(client_fd, reply);
		free(reply);
		return ;
	}
	if (client_fd->client_info.nick)
		free(client_fd->client_info.nick);
	client_fd->client_info.nick = ft_strdup(message->params[0]);
}