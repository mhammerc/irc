#include "../irc.h"

/*
** Action OPER.
** For now, username and password are both 'admin'.
** TODO: get credentials from configuration file
*/
void	command_func_oper(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository	*client;

	client = app->fds + client_fd;
	if (message->params_size < 2)
	{
		client_reply(client, ERR_NEEDMOREPARAMS, "OPER :Not enough parameters");
		return ;
	}
	if (ft_strcmp(message->params[0], "admin") != 0
		|| ft_strcmp(message->params[1], "admin") != 0)
	{
		client_reply(client, ERR_PASSWDMISMATCH, ":Password incorrect");
		return ;
	}
	client->client_info.mode |= CLIENT_MODE_OPERATOR;
	client_reply(client, RPL_YOUROPER, ":You are now an IRC operator");
}