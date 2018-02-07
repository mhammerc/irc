#include "../irc.h"

void		command_func_user(t_app *app, int client_fd)
{
	(void)app;
	(void)client_fd;
	printf("USER :D\n");
}