#include "irc.h"

void     error(char *str)
{
	perror(str);
	exit(1);
}