#include "parse.h"
#include "../minishell.h"

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

static int heredoc_child_process(const char *delimiter, int expand, const char *filename)
{
	char *line;
	int fd;

	// Setup signals for heredoc
	setup_signals_heredoc();

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return -1;

	while (1)
	{
		// Check for signal interruption
		if (g_signal_num == SIGINT)
		{
			close(fd);
			unlink(filename);  // Clean up temp file
			exit(130);  // Exit with SIGINT status
		}

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
			char *line_copy = line;
			char *expanded = handle_hd_line(&line_copy);
			if (expanded)
			{
				free(line);
				line = expanded;
			}
			else
			{
				free(line);
				close(fd);
				return -1;
			}
		}

		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}

	close(fd);
	exit(0);  // Success
}

int handle_heredoc(const char *delimiter, int expand, char **out_filename)
{
	static int hdoc_count = 0;
	char *fname = heredoc_generate_filename(&hdoc_count);
	pid_t pid;
	int status;

	if (!fname || !delimiter || !out_filename)
		return -1;

	// Reset signal before fork
	g_signal_num = 0;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(fname);
		return -1;
	}

	if (pid == 0)
	{
		// Child process handles heredoc input
		heredoc_child_process(delimiter, expand, fname);
		// This should not be reached (child exits in heredoc_child_process)
		exit(1);
	}
	else
	{
		// Parent process waits for child
		waitpid(pid, &status, 0);

		// Check if child was interrupted by signal
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			// Child was interrupted by Ctrl-C
			free(fname);
			g_signal_num = SIGINT;  // Signal that heredoc was interrupted
			return -1;
		}

		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			// Child exited due to SIGINT
			free(fname);
			g_signal_num = SIGINT;
			return -1;
		}

		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{
			// Child failed for other reasons
			free(fname);
			unlink(fname);
			return -1;
		}

		// Success - child completed heredoc normally
		*out_filename = fname;
		add_temp_file(fname);  // Register for cleanup
		return 0;
	}
}