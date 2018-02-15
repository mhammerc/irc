#include "../irc.h"

static void	print_channels_info(t_app *app, t_fd_repository *client_fd, char *channel_name)
{
	t_list		*iter;
	t_channel	*chan;
	char		*reply;

	iter = app->channels;
	while (iter)
	{
		chan = (t_channel*)iter->content;	
		if (ft_strcmp(chan->name, channel_name) == 0)
		{
			ft_sprintf(&reply, "%s %d %s", chan->name, chan->num_clients, chan->description);
			client_reply(client_fd, RPL_LIST, reply);
			free(reply);
			return ;
		}
		iter = iter->next;
	}
}

/*
** Action LIST.
** Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]
** Target isn't supported here.
*/
void		command_func_list(t_app *app, int _client_fd, t_irc_message *message)
{
	t_fd_repository		*client_fd;
	char				**channels;
	t_list				*iter;
	t_channel			*iter_channel;
	int					i;

	client_fd = app->fds + _client_fd;
	if (message->params_size > 0 && message->params[0])
	{
		channels = ft_strsplit(message->params[0], ',');
	}
	else
	{
		channels = malloc((ft_lstcount(app->channels) + 1) * sizeof(char*));
		i = 0;
		iter = app->channels;
		while (iter)
		{
			iter_channel = (t_channel*)iter->content;
			channels[i] = ft_strdup(iter_channel->name);
			++i;
			iter = iter->next;
		}
		channels[i] = 0;
	}
	i = 0;
	while (channels[i])
	{
		print_channels_info(app, client_fd, channels[i]);
		free(channels[i]);
		++i;
	}
	free(channels);
	client_reply(client_fd, RPL_LISTEND, ":End of LIST");
}