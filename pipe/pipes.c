/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:44:31 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 16:45:19 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	child_process(t_shell *shell, int pipefd[2], int prev_fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, 0);
		close(prev_fd);
	}
	if (shell->cmds->next)
	{
		dup2(pipefd[1], 1);
		close(pipefd[1]);
	}
	handle_redirs(shell);
	if (!check_builtin(shell))
		check_exec(shell);
	exit(127);
}

static void	parent_process(t_shell *shell, int pipefd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (shell->cmds->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

void	handle_pipes(t_shell *shell)
{
	int			pid;
	int			pipefd[2];
	int			prev_fd;
	t_cmdnode	*tmp;

	prev_fd = -1;
	tmp = shell->cmds;
	while (shell->cmds)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
			child_process(shell, pipefd, prev_fd);
		else
			parent_process(shell, pipefd, &prev_fd);
		shell->cmds = shell->cmds->next;
	}
	while (wait(NULL) > 0)
		;
	free_cmd_list(tmp);
}
