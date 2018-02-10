#include "../irc.h"

void		command_func_nick(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository		*_client_fd;

	_client_fd = app->fds + client_fd;
	_client_fd->client_info.nick = ft_strdup(*message->params);
}