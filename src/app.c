#include "irc.h"

/*
** Register the commands into the app here.
*/
void	register_base_commands(t_app *app)
{
	command_register(app, "NICK", command_func_nick);
	command_register(app, "USER", command_func_user);
	command_register(app, "LIST", command_func_list);
	command_register(app, "OPER", command_func_oper);
	command_register(app, "MODE", command_func_mode);
	command_register(app, "QUIT", command_func_quit);
	command_register(app, "JOIN", command_func_join);
}

static void	register_base_channels(t_app *app)
{
	channel_create(app, "!general", "General discussions");

	channel_create(app, "!flood", "Non-moderated discussions");
}

/*
** Init the app global variable.
** Set maxfd and prepare the fd array.
*/
void     init_app(t_app *app)
{
	struct rlimit   rlp;
	int             ret;
	int				i;

	ret = getrlimit(RLIMIT_NOFILE, &rlp);
	if (ret == -1)
		error("getrlimit failed");
	app->maxfd = rlp.rlim_cur;
	app->fds = (t_fd_repository*)malloc(sizeof(t_fd_repository) * app->maxfd);
	if (app->fds == NULL)
		error("malloc failed");
	i = 0;
	while (i < app->maxfd)
	{
		fd_repo_clean(app->fds + i);
		++i;
	}
	register_base_commands(app);
	register_base_channels(app);
}

t_app    *get_app()
{
	static t_app    app = {};

	return (&app);
}