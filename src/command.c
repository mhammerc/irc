#include "irc.h"

void				command_register(t_app *app, char *name, t_irc_command_func func)
{
	t_irc_command	command;
	t_list			*node;

	command.name = ft_strdup(name);
	command.func = func;
	node = ft_lstnew(&command, sizeof(t_irc_command));
	ft_lstadd(&app->irc_commands, node);
}

t_irc_command		*command_search(t_app *app, char *name)
{
	t_list			*iter;
	t_irc_command	*command;

	iter = app->irc_commands;
	while (iter)
	{
		command = (t_irc_command*)iter->content;
		if (strcmp(command->name, name) == 0)
			return (command);
		iter = iter->next;
	}
	return (NULL);
}