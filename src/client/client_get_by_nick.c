#include "../irc.h"

t_fd_repository		*client_get_by_nick(t_app *app, char *nick)
{
	int		i;

	i = 0;
	while (i < app->maxfd)
	{
		if (ft_strcmp(app->fds[i].client_info.nick, nick) == 0)
		{
			return app->fds + i;
		}
	}
	return (NULL);
}