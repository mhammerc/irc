#include "irc.h"

void fd_repo_clean(t_fd_repository *fd)
{
	fd->type = FD_FREE;
	fd->fct_read = NULL;
	fd->fct_write = NULL;
	circular_buffer_init(&fd->buf_read, BUF_SIZE);
	circular_buffer_init(&fd->buf_write, BUF_SIZE);
	ft_bzero(&fd->client_info, sizeof(t_client_info));
}