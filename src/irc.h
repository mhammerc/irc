#ifndef IRC_H
# define IRC_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include <sys/select.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "../libft/libft.h"

# define FD_FREE 0
# define FD_SERV 1
# define FD_CLIENT 2

# define BUF_SIZE 4096

# define MAX(a, b) ((a > b) ? a : b)
# define MIN(a, b) ((a > b) ? b : a)
# define ABS(a)    ((a > 0) ? a : -a)

typedef struct s_app        t_app;

/*
** CIRCULAR BUFFER
*/
typedef struct				s_circular_buffer
{
	char					*buffer;
	size_t					buffer_size;
	/*
	** begin: read cursor
	** end: write cursor
	*/
	size_t					begin;
	size_t					end;
}							t_circular_buffer;

void						circular_buffer_init(t_circular_buffer *b, size_t size);
void						circular_buffer_destroy(t_circular_buffer *b);
void						circular_buffer_write(t_circular_buffer *b, char *data, size_t data_size);
size_t						circular_buffer_read(t_circular_buffer *b, char delimiter, char **result);

/*
** CLIENT MANAGEMENT
** (with fd)
** (clients & server)
*/

typedef struct              s_client_info
{
    char                    *username;
    char                    *nick;
    char                    *hostname;
    char                    *realname;
    int                     mode;
}                           t_client_info;

typedef struct              s_fd_repository
{
    int                     type;
    void                    (*fct_read)();
    void                    (*fct_write)();
	t_circular_buffer		buf_read;
	t_circular_buffer		buf_write;
    t_client_info           client_info;
}                           t_fd_repository;

void						fd_repo_clean(t_fd_repository *fd);
void						client_reply(t_fd_repository *client, char *reply);
void            			client_write(t_app *app, int client_fd, t_fd_repository *client);
void            			client_read(t_app *app, int client_fd);

/*
** IRC MESSAGES PARSER
** Compliant to RFC 2812
** https://tools.ietf.org/html/rfc2812#section-2.3
*/
typedef struct              s_irc_message_prefix
{
    char                    *servername;
    char                    nickname[9];
    char                    *user;
    char                    *host;
}                           t_irc_message_prefix;

typedef struct              s_irc_message
{
	char					*original;
    t_irc_message_prefix    prefix;
    char                    *command;
    char                    *params[15];
    size_t                  params_size;
}                           t_irc_message;

t_irc_message               *irc_message_parse(char *message, size_t message_len);
void                        irc_message_destroy(t_irc_message **message);

/*
** REPLIES
*/
# include "responses.h"


/*
** IRC COMMANDS FUNCTIONS
*/
typedef void                (*t_irc_command_func)(t_app *app, int client_fd, t_irc_message *message);
typedef struct              s_irc_command
{
    char                    *name;
    t_irc_command_func      func;
}                           t_irc_command;

void                        command_register(t_app *app, char *name, t_irc_command_func func);
t_irc_command               *command_search(t_app *app, char *name);

void		                command_func_nick(t_app *app, int client_fd, t_irc_message *message);
void		                command_func_user(t_app *app, int client_fd, t_irc_message *message);
void		                command_func_list(t_app *app, int client_fd, t_irc_message *message);

/*
** MACRO APP MANAGEMENT
*/
struct              s_app
{
    t_fd_repository         *fds;
    int                     port;
    int                     maxfd;
    int                     max;
    int                     r;
    fd_set                  fd_read;
    fd_set                  fd_write;
    t_list                  *irc_commands;
};

void						error(char *str);
void						init_app(t_app *app);
t_app    					*get_app();

void						server_start(t_app *app);
void						loop_start(t_app *app);


#endif