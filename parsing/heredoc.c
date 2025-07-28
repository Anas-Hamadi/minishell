#include "parse.h"

static char *heredoc_generate_filename(int *hdoc_count)
{
	char *fn = malloc(64);
	char *pid_str, *count_str;
	size_t fn_size = 64;
	size_t fn_len = 0;

	if (!fn)
		return NULL;
	fn[0] = '\0';

	// Build: /tmp/.heredoc_<pid>_<count>
	if (!safe_strcat_realloc(&fn, &fn_size, &fn_len, "/tmp/.heredoc_"))
		return NULL;

	pid_str = ft_itoa_simple((int)getpid());
	if (!pid_str)
	{
		free(fn);
		return NULL;
	}
	if (!safe_strcat_realloc(&fn, &fn_size, &fn_len, pid_str))
	{
		free(pid_str);
		return NULL;
	}
	free(pid_str);

	if (!safe_charcat_realloc(&fn, &fn_size, &fn_len, '_'))
		return NULL;

	count_str = ft_itoa_simple((*hdoc_count)++);
	if (!count_str)
	{
		free(fn);
		return NULL;
	}
	if (!safe_strcat_realloc(&fn, &fn_size, &fn_len, count_str))
	{
		free(count_str);
		return NULL;
	}
	free(count_str);

	return fn;
}

int handle_heredoc(const char *delimiter, int expand, char **out_filename)
{
	static int hdoc_count = 0; // static instead of global
	char *fname = heredoc_generate_filename(&hdoc_count);
	char *line;
	int fd;

	if (!fname || !delimiter || !out_filename)
		return -1;

	fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(fname);
		return -1;
	}

	while (1)
	{
		line = readline("> ");
		if (!line) /* user pressed Ctrl+D */
			break;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		if (expand)
		{
			line = handle_hd_line(&line);
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	*out_filename = fname;
	return 0;
}