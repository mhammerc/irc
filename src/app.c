#include "irc.h"

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
}

t_app    *get_app()
{
	static t_app    app = {};

	return (&app);
}