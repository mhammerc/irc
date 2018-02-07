#include "../irc.h"

void		command_func_nick(t_app *app, int client_fd)
{
	(void)app;
	(void)client_fd;
	printf("NICK :D\n");
}