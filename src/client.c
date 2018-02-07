#include "irc.h"

void            client_write(t_app *app, int client_fd)
{
	(void)app;
	(void)client_fd;
	//send(client_fd, "hello\n", 6, 0);
}

void            client_read(t_app *app, int client_fd)
{
	size_t			len;
	char			buffer[BUF_SIZE + 1];
	char			*message_str;
	t_irc_message	*message;
	t_irc_command	*command;

	len = recv(client_fd, buffer, BUF_SIZE, 0);
	if (len <= 0)
	{
		close(client_fd);
		fd_repo_clean(app->fds + client_fd);
		printf("Client #%d disconnected.\n", client_fd);
		return ;
	}

	buffer[len] = 0;
	circular_buffer_write(&app->fds[client_fd].buf_read, buffer, len);

	message_str = NULL;
	len = 0;
	while (1)
	{
		len = circular_buffer_read(&app->fds[client_fd].buf_read, '\n', &message);
		if (len == 0)
			break;
		message = irc_message_parse(message_str);
		command = command_search(app, message->command);
		if (command)
			command->func(app, client_fd, message);
		irc_message_destroy(&message);
	}
}