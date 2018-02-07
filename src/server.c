#include "irc.h"

static void				server_accept(t_app *app, int socket_fd)
{
  int			        client_fd;
  struct sockaddr_in	csin;
  socklen_t		        csin_len;

  csin_len = sizeof(struct sockaddr_in);
  client_fd = accept(socket_fd, (struct sockaddr*)&csin, &csin_len);
  if (client_fd == -1)
	error("accept failed");

  printf("New client #%d from %s:%d\n", client_fd,
	inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));

  fd_repo_clean(app->fds + client_fd);
  app->fds[client_fd].type = FD_CLIENT;
  app->fds[client_fd].fct_read = client_read;
  app->fds[client_fd].fct_write = client_write;
}

/*
** Create the server.
** Create a TCP socket, bind it to the port.
** Listen and wait!!
** Note: PF_INET == AF_INET
*/
void					server_start(t_app *app)
{
	int					ret;
	int                 socket_fd;
	struct sockaddr_in  sin;
	struct protoent     *pe;

	pe = getprotobyname("tcp");
	if (!pe)
		error("getprotobyname failed");
	socket_fd = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (socket_fd == -1)
		error("socket failed");
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htons(INADDR_ANY);
	sin.sin_port = htons(app->port);
	ret = bind(socket_fd, (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
	if (ret == -1)
		error("bind failed");
	ret = listen(socket_fd, 42);
	if (ret == -1)
		error("listen failed");
	app->fds[socket_fd].type = FD_SERV;
	app->fds[socket_fd].fct_read = server_accept;
}