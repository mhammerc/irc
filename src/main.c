#include "irc.h"

static void     print_usage()
{
	fprintf(stderr, "Usage: ./serveur <port>\n");
	exit(1);
}

int main(int argc, char **argv)
{
	t_app   *app;

	app = get_app();
	if (argc != 2)
		print_usage();
	app->port = atoi(argv[1]);
	if (app->port <= 0)
		print_usage();
	
	init_app(app);
	server_start(app);
	loop_start(app);
}