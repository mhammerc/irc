#include "irc.h"

/*
** An IRC message but be:
** [:<prefix>] command [param1[ param2 [..]]]
** Ignore prefix for now.
** Easy rule from IRF RFC: prefix is always prefixed by ':'. And that ':' is
** always on the very first byte.
*/
t_irc_message	*irc_message_parse(char *message)
{
	char			*next_space;
	t_irc_message	*parsed;

	parsed = (t_irc_message*)malloc(sizeof(t_irc_message));
	/*
	** Prefix
	*/
	if (*message == ':')
	{

	}
	next_space = ft_strchr(message, ' ');
	parsed->command = (char*)malloc(sizeof(char) * (next_space - message + 1));
	ft_memcpy(parsed->command, message, next_space - message);
	parsed->command[next_space - message] = 0;
	return (parsed);
}

void			irc_message_destroy(t_irc_message **message)
{
	free((*message)->command);
	free(*message);
	*message = NULL;
}