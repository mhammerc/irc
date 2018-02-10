#include "../irc.h"

void		command_func_list(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository		*_client_fd;

	_client_fd = app->fds + client_fd;
	(void)message;
	char	*result = NULL;
	ft_sprintf(&result, ":127.0.0.1 322 !general #2 :General chan\r\n:127.0.0.1 323 :End of LIST\r\n", _client_fd->client_info.username);
	circular_buffer_write(&_client_fd->buf_write, result, ft_strlen(result));
	free(result);
}