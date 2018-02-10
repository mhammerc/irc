#include "irc.h"

/*
** Insert a reply into the client write buffer.
** 
** client: structure to clients data
** msg: reply to send. Must be null-terminated
*/
void			client_reply(t_fd_repository *client, char *msg)
{
	if (!client || !msg)
	{
		fprintf(stderr, "Received NULL parameter on %s:%s", __FILE__, __LINE__);
		return ;
	}
	circular_buffer_write(&client->buf_write, msg, strlen(msg));
}

void            client_write(t_app *app, int client_fd, t_fd_repository *client)
{
	size_t	len;
	char	*str;

	(void)app;
	len = circular_buffer_read(&client->buf_write, '\n', &str);
	if (len > 0)
	{
		send(client_fd, str, len, 0);
		printf("Send %s\n", str);
		free(str);
	}
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
		len = circular_buffer_read(&app->fds[client_fd].buf_read, '\n', &message_str);
		if (len == 0)
			break;
		message = irc_message_parse(message_str, len);
		command = command_search(app, message->command);
		if (command)
			command->func(app, client_fd, message);
		else
			printf("CMD not found. |%s| |%s|", message->command, message_str);
		irc_message_destroy(&message);
	}
}