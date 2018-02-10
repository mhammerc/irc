#include "irc.h"

void	loop_start(t_app *app)
{
	int	i = 0;
	while (1)
	{
		i = 0;
		app->max = 0;
		FD_ZERO(&app->fd_read);
		FD_ZERO(&app->fd_write);
		while (i < app->maxfd)
		{
			if (app->fds[i].type == FD_FREE)
			{
				++i;
				continue;
			}

			FD_SET(i, &app->fd_read);
			FD_SET(i, &app->fd_write);
			app->max = MAX(app->max, i);
			++i;
		}

		app->r = select(app->max + 1, &app->fd_read, &app->fd_write, NULL, NULL);

		i = 0;
		while ((i < app->maxfd) && (app->r > 0))
		{
			if (FD_ISSET(i, &app->fd_read))
				app->fds[i].fct_read(app, i);
			if (FD_ISSET(i, &app->fd_write))
				app->fds[i].fct_write(app, i, app->fds + i);
			if (FD_ISSET(i, &app->fd_read) || FD_ISSET(i, &app->fd_write))
				--app->r;
			++i;
		}
	}
}