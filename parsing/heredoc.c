/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:15:23 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/02 20:03:50 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

static int	handle_child_signals(pid_t pid, char *fname, int status)
{
	(void)pid;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		free(fname);
		g_signal_num = SIGINT;
		return (-1);
	}
	return (0);
}

static int	fork_heredoc_process(t_shell *shell, const char *delimiter,
		int *expand, const char *fname)
{
	pid_t	pid;

	g_signal_num = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		heredoc_child_process(delimiter, expand, fname, shell);
		exit(1);
	}
	return (pid);
}

int	handle_heredoc(t_shell *shell, const char *delimiter, int *expand,
		char **out_filename)
{
	static int	hdoc_count;
	char		*fname;
	pid_t		pid;
	int			status;

	hdoc_count = 0;
	fname = heredoc_generate_filename(shell, &hdoc_count);
	if (!fname || !delimiter || !out_filename)
		return (-1);
	pid = fork_heredoc_process(shell, delimiter, expand, fname);
	if (pid == -1)
	{
		free(fname);
		return (-1);
	}
	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (handle_child_signals(pid, fname, status) < 0)
		return (-1);
	*out_filename = fname;
	return (0);
}
