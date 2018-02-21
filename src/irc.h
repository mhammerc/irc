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

# include "hash_table/hash_table.h"
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

# define CLIENT_MODE_AWAY 2
# define CLIENT_MODE_WALLOPS 4
# define CLIENT_MODE_INVISIBLE 8
# define CLIENT_MODE_RESTRICTED 16
# define CLIENT_MODE_OPERATOR 32
# define CLIENT_MODE_LOCAL_OPERATOR 64

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
void						client_reply(t_fd_repository *client, char *numeric_reply, char *reply);
void						client_reply_with_origin(t_fd_repository *dest, t_fd_repository *src, char *numeric_reply, char *msg);
void						client_reply_raw(t_fd_repository *client, char *msg);
void            			client_write(t_app *app, int client_fd, t_fd_repository *client);
void            			client_read(t_app *app, int client_fd);
t_fd_repository 			*client_get_by_nick(t_app *app, char *nick);

/*
** CHANNELS MANAGEMENT
*/
typedef struct				s_channel
{
	char					*name;
	char					*description;
	int						mode;
	t_list					*clients;
	size_t					num_clients;
}							t_channel;

# define CHANNEL_MODE_ANONYMOUS (1 << 0)
# define CHANNEL_MODE_INVITE_ONLY (1 << 1)
# define CHANNEL_MODE_MODERATED (1 << 2)
# define CHANNEL_MODE_NO_OUTSIDE_MESSAGE (1 << 3)
# define CHANNEL_MODE_QUIET (1 << 4)
# define CHANNEL_MODE_PRIVATE (1 << 5)
# define CHANNEL_MODE_SECRET (1 << 6)
# define CHANNEL_MODE_SERVER_REOP (1 << 7)
# define CHANNEL_MODE_TOPIC_SETTABLE_BY_OPERATOR_ONLY (1 << 8)
# define CHANNEL_MODE_KEY (1 << 8)
# define CHANNEL_MODE_USER_LIMIT (1 << 9)

void						channel_register(t_app *app, t_channel *channel);
void						channel_create(t_app *app, char *name, char *description);
void						channel_remove(t_app *app, char *name);
void						channel_free(t_app *app, t_channel **chan);
t_channel					*channel_get(t_app *app, char *name);
int							channel_add_client(t_app *app, char *channel_name, t_fd_repository *client);
void						channel_remove_client(t_app *app, char *channel_name, t_fd_repository *client);
int							channel_have_client(t_app *app, char *channel_name, t_fd_repository *client);
void						channel_broadcast(t_app *app, char *channel_name, char *msg);

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
void		                command_func_oper(t_app *app, int client_fd, t_irc_message *message);
void						command_func_mode(t_app *app, int client_fd, t_irc_message *message);
void						command_func_quit(t_app *app, int client_fd, t_irc_message *message);

void						command_func_join(t_app *app, int client_fd, t_irc_message *message);
void						command_func_part(t_app *app, int client_fd, t_irc_message *message);
void						command_func_topic(t_app *app, int client_fd, t_irc_message *message);
void						command_func_names(t_app *app, int client_fd, t_irc_message *message);
void						command_func_invite(t_app *app, int client_fd, t_irc_message *message);
void						command_func_kick(t_app *app, int client_fd, t_irc_message *message);

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
	t_list					*channels;
};

void						error(char *str);
void						init_app(t_app *app);
t_app    					*get_app();

void						server_start(t_app *app);
void						loop_start(t_app *app);


#endif