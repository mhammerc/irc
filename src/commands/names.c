#include "../irc.h"

/*
** Action NAMES.
**
** Note: Because this IRC server doesn't support RFC 2813, last paramter
** <target> isn't supported here.
**
** TODO: Support =, *, @ channel prefix and @, + channel prefix.
** TODO: beautify the function
*/
void	command_func_names(t_app *app, int client_fd, t_irc_message *message)
{
	t_fd_repository		*client;
	char				*reply;
	char				**channels;
	t_channel			*channel;
	int					i;
	t_list				*iter;
	t_fd_repository		*channel_client;


	client = app->fds + client_fd;
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
			channel = (t_channel*)iter->content;
			channels[i] = ft_strdup(channel->name);
			++i;
			iter = iter->next;
		}
		channels[i] = 0;
	}
	i = 0;
	while (channels[i])
	{
		channel = channel_get(app, channels[i]);
		if (!channel)
		{
			free(channels[i]);
			++i;
			continue ;
		}
		reply = (char*)malloc(10 * channel->num_clients + strlen(channel->name) + 3);
		reply[0] = '\0';
		ft_strcat(reply, channel->name);
		ft_strcat(reply, " :");
		iter = channel->clients;
		while (iter)
		{
			channel_client = (t_fd_repository*)iter->content;
			ft_strcat(reply, " ");
			ft_strcat(reply, channel_client->client_info.nick);
			iter = iter->next;
		}	
		client_reply(client, RPL_NAMREPLY, reply);
		free(reply);
		free(channels[i]);
		++i;
	}	
	free(channels);
	client_reply(client, RPL_ENDOFNAMES, ":End of NAMES list");
}