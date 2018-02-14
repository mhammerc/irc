#include "../irc.h"

/*
** Action USER
** TODO: ERR_ALREADYREGISTRED
** TODO: save mode
*/
void		command_func_user(t_app *app, int _client_fd, t_irc_message *message)
{
	t_fd_repository	*client_fd;
	char			*reply;

	client_fd = app->fds + _client_fd;
	reply = NULL;
	if (message->params_size < 4)
	{
		client_reply(client_fd, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
		return ;
	}
	client_fd->client_info.username = ft_strdup(message->params[0]);
	client_fd->client_info.realname = ft_strdup(message->params[3]);
	ft_sprintf(&reply, "%s %s%s%s%s",
	":Welcome to the Internet Relay Network",
	client_fd->client_info.nick, "!~", client_fd->client_info.username, "@localhost");
	client_reply(client_fd, RPL_WELCOME, reply);
	free(reply);
	client_reply(client_fd, RPL_YOURHOST, ":Your host is localhost, running version 0.0.1a");
	client_reply(client_fd, RPL_CREATED, ":This server was created 31/02/18");
	client_reply(client_fd, RPL_MYINFO, "localhost 0.0.1a aoOirw abeiIklmnoOpqrRstv");
}