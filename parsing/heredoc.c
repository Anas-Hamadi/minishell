#include "../minishell.h"
#include "parse.h"

static char	*heredoc_generate_filename(struct s_shell *shell, int *hdoc_count)
{
	char	*fn;
	size_t	fn_size;
	size_t	fn_len;

	fn = malloc(64);
	char *pid_str, *count_str;
	fn_size = 64;
	fn_len = 0;
	if (!fn)
		return (NULL);
	fn[0] = '\0';
	// Build: /tmp/.heredoc_<pid>_<count>
	if (!safe_strcat_realloc(&fn, &fn_size, &fn_len, "/tmp/.heredoc_"))
		return (NULL);
	pid_str = ft_itoa_simple((int)getpid());
	if (!pid_str)
	{
		free(fn);
		return (NULL);
	}
	if (!safe_strcat_realloc(&fn, &fn_size, &fn_len, pid_str))
	{
		free(pid_str);
		return (NULL);
	}
	free(pid_str);
	if (!safe_charcat_realloc(&fn, &fn_size, &fn_len, '_'))
		return (NULL);
	count_str = ft_itoa_simple((*hdoc_count)++);
	if (!count_str)
	{
		free(fn);
		return (NULL);
	}
	if (!safe_strcat_realloc(&fn, &fn_size, &fn_len, count_str))
	{
		free(count_str);
		return (NULL);
	}
	free(count_str);
	// Register temp file for cleanup
	add_temp_file(shell, fn);
	return (fn);
}

static int	heredoc_child_process(const char *delimiter, int expand,
		const char *filename, t_shell *shell)
{
	char	*line;
	int		fd;
	char	*line_copy;
	char	*expanded;

	// Setup signals for heredoc
	// setup_signals_heredoc();
	signal(SIGINT,SIG_DFL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line) /* user pressed Ctrl+D */
		{
            write (STDOUT_FILENO, "here-document delimited by end-of-file\n", 39);
            break ;
        }
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			line_copy = line;
			expanded = handle_hd_line(shell, &line_copy);
			if (expanded)
			{
				free(line);
				line = expanded;
			}
			else
			{
				free(line);
				close(fd);
				return (-1);
			}
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	exit(0); // Success
}

int	handle_heredoc(t_shell *shell, const char *delimiter, int expand,
		char **out_filename)
{
	static int hdoc_count = 0;
	char *fname = heredoc_generate_filename(shell, &hdoc_count);
	pid_t pid;
	int status;

	if (!fname || !delimiter || !out_filename)
		return (-1);

	// Reset signal before fork
	g_signal_num = 0;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(fname);
		return (-1);
	}

	if (pid == 0)
	{
		// Child process handles heredoc input
		heredoc_child_process(delimiter, expand, fname, shell);
		// This should not be reached (child exits in heredoc_child_process)
		exit(1);
	}
	else
	{
		// Parent process waits for child
		waitpid(pid, &status, 0);
		setup_signals_interactive();

		// Check if child was interrupted by signal
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			// Child was interrupted by Ctrl-C
			write(STDOUT_FILENO, "\n", 1);
			free(fname);
			g_signal_num = SIGINT; // Signal that heredoc was interrupted
			return (-1);
		}

		// Success - child completed heredoc normally
		*out_filename = fname;
		// Note: temp file already registered in heredoc_generate_filename
		return (0);
	}
}