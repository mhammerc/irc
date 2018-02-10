#include "../irc.h"

void		command_func_user(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository		*_client_fd;

	_client_fd = app->fds + client_fd;
	_client_fd->client_info.username = ft_strdup(*message->params);
	_client_fd->client_info.hostname = ft_strdup(*(message->params + 2));
	_client_fd->client_info.realname = ft_strdup(*(message->params + 3));

	char	*result = NULL;
	ft_sprintf(&result, ":127.0.0.1 001 %s Welcome on Martin's IRC Server!\r\n", _client_fd->client_info.username);
	client_reply(_client_fd, result);
	free(result);
}