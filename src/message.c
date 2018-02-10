#include "irc.h"

/*
** Perform basic tests on message, according to RFC 2812.
** Tests are ofc not comprehensive.
**
** message:  string compliant to RFC 2812
** len:      length of 'message'
** return:   1 if the message pass tests, 0 if not.
*/
static int		base_validity_tests(char *message, size_t len)
{
	if (len < 3)
		return (0);
	if (message[len - 1] != '\n' || message[len - 2] != '\r')
		return (0);
	return (1);
}

/*
** Slightly edit message to make it easer to work on.
**
**  - Remove \r\n at end
*/
static void		refactor_message(char *message, size_t len)
{
	message[len - 1] = 0;
	message[len - 2] = 0;
}

/*
** Read the prefix in the message.
** According to RFC 2812, the prefix is optional.
** If there is a prefix, the first byte must be equal to ':'.
**
** prefix = servername / ( nickname [ [ "!" user ] "@" host ] )
**
** parsed:  structure to save parsed data
** message: string compliant to RFC 2812 to parse
** return:  a pointer on the first byte after prefix in 'message' or
**          0 if parsing error.
*/
static char		*read_prefix(t_irc_message *parsed, char *message)
{
	char	*return_value;

	(void)parsed;
	if (*message != ':')
		return (message);
	return_value = ft_strchr(message, ' ');		
	if (return_value == 0)
		return (0);
	return (return_value);
}

/*
** Read the command in the message.
** 
** command = 1*letter / 3digit
**
** parsed:  structure to save parsed data
** message: string compliant to RFC 2812 to parse. It will be located on
**          the command directly. (because of read_prefix()). There may be
**          spaces before the command.
** return:  a pointer on the first byte after command in 'message' or 0 if
**          parsing error.
*/
static char		*read_command(t_irc_message *parsed, char *message)
{
	char	*command_end;
	size_t	command_length;
	size_t	i;
	int		flags; // first bit: str have letters. second bit: str have digits

	while (*message == ' ')
		++message;
	if (!ft_isalnum(*message))
		return (0);
	command_end = ft_strchrnul(message, ' ');
	command_length = command_end - message;
	parsed->command = ft_strndup(message, command_length);
	if (!parsed->command)
		return (NULL);
	/* Check bytes are valid */
	i = 0;
	flags = 0;
	while (parsed->command[i])
	{
		if (!ft_isalpha(parsed->command[i]) && !ft_isdigit(parsed->command[i]))
			return (NULL);
		if (ft_isalpha(parsed->command[i]))
			flags |= 1 << 0;
		if (ft_isdigit(parsed->command[i]))
			flags |= 1 << 1;
		++i;
	}
	if (flags == 3 || (flags == 2 && i != 3)) // we cannot mix digits and letters
		return (NULL);
	return (command_end);
}

/*
** Check if a char is spcrlfcl
**
** nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
**                  ; any octet except NUL, CR, LF, " " and ":"
*/
static int		is_spcrlfcl(char c)
{
	if (c == '\0' || c == '\r' || c == '\n' || c == ' ' || c == ':')
		return (1);
	return (0);
}

/*
** Read params in the message.
**
** params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
**            =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

** nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
**                  ; any octet except NUL, CR, LF, " " and ":"
** middle     =  nospcrlfcl *( ":" / nospcrlfcl )
** trailing   =  *( ":" / " " / nospcrlfcl )

** parsed:   structure to save parsed data
** message:  string compliant to RFC 2812 to parse. It will be located on params
**           . (because of read_command()). there may be spaces before the
**           first param.
** return:   0 if parsing error, else non-zero value.
*/
static char		*read_params(t_irc_message *parsed, char *message)
{
	char	*iter;
	char	*iter2;
	size_t	num_params;

	iter = message;
	num_params = 0;
	while (*iter)
	{
		while (*iter == ' ')
			++iter;
		if (!is_spcrlfcl(*iter))
		{
			iter2 = iter;
			while (!is_spcrlfcl(*iter))
				++iter;
			parsed->params[num_params] = ft_strndup(iter2, iter - iter2);
			++num_params;
		}
		if ((*iter == ':'))
		{
			parsed->params[num_params] = ft_strdup(iter + 1);
			++num_params;
			break;
		}
	}
	parsed->params_size = num_params;
	return (iter);
}

/*
** An IRC message is (from RFC 2812) :
** message = [ ":" prefix SPACE ] command [ params ] crlf
**
** Note: this parsing is flexible. The RFC 2812 is very strict on spaces,
** and things like that. We silently ignore errors like too much spaces.
**
** message: string compliant to RFC 2812 to parse
** len: length of 'message'
** return: structure with parsed data
**
** TODO: also parse the prefix
*/
t_irc_message	*irc_message_parse(char *_message, size_t len)
{
	char			*message;
	t_irc_message	*parsed;

	
	if (!base_validity_tests(_message, len))
		return (NULL);
	parsed = (t_irc_message*)malloc(sizeof(t_irc_message));	
	ft_bzero(parsed, sizeof(t_irc_message));
	parsed->original = ft_strdup(_message);
	message = parsed->original;
	refactor_message(message, len);
	message = read_prefix(parsed, message);
	if (!message)
	{
		irc_message_destroy(&parsed);
		return (NULL);
	}
	message = read_command(parsed, message);
	if (!message)
	{
		irc_message_destroy(&parsed);
		return (NULL);
	}
	message = read_params(parsed, message);
	if (!message)
	{
		irc_message_destroy(&parsed);
		return (NULL);
	}
	return (parsed);
}

void			irc_message_destroy(t_irc_message **message)
{
	t_irc_message	*_message;
	size_t			i;

	if (!message || !*message)
		return;
	_message = *message;
	if (_message->original)
		free(_message->original);
	if (_message->prefix.servername)
		free(_message->prefix.servername);
	if (_message->prefix.user)
		free(_message->prefix.user);
	if (_message->prefix.host)
		free(_message->prefix.host);
	if (_message->command)
		free(_message->command);
	i = 0;
	while (i < 15)
	{
		if (_message->params[i] == 0)
			break;
		free(_message->params[i]);
		++i;
	}
	free(_message);
	*message = NULL;
}