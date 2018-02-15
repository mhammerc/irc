#include "../irc.h"

void	channel_free(t_app *app, t_channel **_chan)
{
	t_channel	*chan;

	(void)app;
	chan = *_chan;
	if (chan->description)
		free(chan->description);
	free(chan->name);
	free(chan);
	*_chan = NULL;
}