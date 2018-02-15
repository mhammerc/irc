#include "../irc.h"

/*
** Create a channel and register it in channel list.
**
** This is a low-level function. You have to take care of not inserting two
** channels with the same name.
*/
void	channel_create(t_app *app, char *name, char *description)
{
	t_channel	channel;

	ft_bzero(&channel, sizeof(t_channel));
	channel.name = ft_strdup(name);
	if (description)
		channel.description = ft_strdup(description);
	channel_register(app, &channel);
}