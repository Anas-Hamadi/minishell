/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:19:22 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/03 17:13:02 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_child_status(t_shell *shell, int status, int waited_pid,
		int last_pid)
{
	int	sig;

	if (waited_pid == last_pid)
	{
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->exit_code = 128 + WTERMSIG(status);
			if (shell->exit_code == 130)
				write(STDERR_FILENO, "\n", 1);
			else if (shell->exit_code == 131)
				write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		}
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == 2)
			write(STDERR_FILENO, "\n", 1);
		else if (sig == 3)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	}
}

void	wait_for_children(t_shell *shell, int last_pid)
{
	int	status;
	int	waited_pid;

	waited_pid = wait(&status);
	while (waited_pid > 0)
	{
		handle_child_status(shell, status, waited_pid, last_pid);
		waited_pid = wait(&status);
	}
}

void	exec_with_path(t_shell *shell, char *full_path, char **env_array)
{
	if (access(full_path, X_OK) == 0)
	{
		execve(full_path, shell->cmds->argv, env_array);
		perror("execve");
	}
	else
	{
		ft_putstr_fd(RED "minishell: " RESET, 2);
		ft_putstr_fd(shell->cmds->argv[0], 2);
		ft_putendl_fd(": Permission denied or file not found" RESET, 2);
	}
}
