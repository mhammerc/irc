#include "../irc.h"

void	channel_broadcast(t_app *app, char *channel_name, char *msg)
{
	t_list		*iter;
	t_channel	*ch;
	
	iter = app->channels;
	ch = NULL;
	while (iter)
	{
		ch = (t_channel*)iter->content;
		if (ft_strcmp(ch->name, channel_name) == 0)
		{
			break ;
		}
		ch = NULL;
		iter = iter->next;
	}
	if (!ch)
		return ;
	iter = ch->clients;
	while (iter)
	{
		client_reply_raw((t_fd_repository*)iter->content, msg);
		iter = iter->next;
	}
}