#include "irc.h"

t_channel	create_channel(char *name, char *description)
{
	t_channel	channel;

	channel.name = ft_strdup(name);
	channel.description = ft_strdup(description);
	channel.num_users = 0;
	return (channel);
}