#include "../irc.h"

/*
** Action QUIT.
** TODO: tell others client that this client is now disconnected.
** BUG: message can't send because of the sending loop. Fix that.
**      Connection is done before the sending loop send message.
*/
void		command_func_quit(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository	*client;
	char			*reply;
	char			*quit_message;

	(void)message;
	client = app->fds + client_fd;
	if (message->params_size == 0)
		quit_message = "";
	else
		quit_message = message->params[0];
	ft_sprintf(&reply, "ERROR :Closing Link: %s (\"%s\")", client->client_info.nick, quit_message);
	client_reply_raw(client, reply);
	free(reply);
	shutdown(client_fd, SHUT_RDWR);
	close(client_fd);
	fd_repo_clean(client);
}