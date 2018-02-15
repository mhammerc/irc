#include "../irc.h"

/*
** Register a created channel in the channel list.
**
** This is a low-level function. You have to take care of not inserting
** two channels with the same name.
*/
void	channel_register(t_app *app, t_channel *channel)
{
	ft_lstadd(&app->channels, ft_lstnew(channel, sizeof(t_channel)));
}