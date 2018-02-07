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


typedef struct              s_fd_repository
{
    int                     type;
    void                    (*fct_read)();
    void                    (*fct_write)();
	t_circular_buffer		buf_read;
    char                    buf_write[BUF_SIZE + 1];

}                           t_fd_repository;

typedef struct              s_app
{
    t_fd_repository         *fds;
    int                     port;
    int                     maxfd;
    int                     max;
    int                     r;
    fd_set                  fd_read;
    fd_set                  fd_write;
}                           t_app;

/*
** MACRO APP MANAGEMENT
*/
void						error(char *str);
void						init_app(t_app *app);
t_app    					*get_app();

void						fd_repo_clean(t_fd_repository *fd);

void						server_start(t_app *app);

void						loop_start(t_app *app);

void            			client_write(t_app *app, int client_fd);
void            			client_read(t_app *app, int client_fd);

#endif