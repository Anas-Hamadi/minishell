/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:44:31 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 15:26:58 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_cmd_child(t_shell *shell)
{
	char	*full_path;
	char	**env_array;

	if (!shell->cmds->argv)
		exit(0);
	env_array = list_to_array(shell->envp);
	if (ft_strchr(shell->cmds->argv[0], '/'))
	{
		if (access(shell->cmds->argv[0], X_OK) == 0)
		{
			execve(shell->cmds->argv[0], shell->cmds->argv, env_array);
			perror("execve");
		}
		else
		{
			ft_putstr_fd(RED "minishell: " RESET, 2);
			ft_putstr_fd(shell->cmds->argv[0], 2);
			ft_putendl_fd(": Permission denied or file not found" RESET, 2);
		}
		ft_free(env_array);
		exit(127);
	}
	full_path = find_cmd_path(shell->cmds->argv[0], shell->envp);
	if (!full_path)
	{
		ft_putstr_fd(RED "minishell: command not found: " RESET, 2);
		ft_putendl_fd(shell->cmds->argv[0], 2);
		ft_free(env_array);
		exit(127);
	}
	else
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
	ft_free(env_array);
	free(full_path);
	exit(127);
}

static void	child_process(t_shell *shell, int pipefd[2], int prev_fd)
{
	setup_signals_child();
	if (prev_fd != -1)
	{
		dup2(prev_fd, 0);
		close(prev_fd);
	}
	if (shell->cmds->next)
	{
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (handle_redirs(shell) < 0)
		exit(1);
	if (check_builtin(shell))
		exit(shell->exit_code);
	else
		exec_cmd_child(shell);
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
	int			last_pid;
	int			status;
	int			waited_pid;
	int			tmp2;

	prev_fd = -1;
	tmp = shell->cmds;
	last_pid = -1;
	signal(SIGINT, SIG_IGN);
	while (shell->cmds)
	{
		if (shell->cmds->next)
			pipe(pipefd);
		pid = fork();
		if (pid == 0)
			child_process(shell, pipefd, prev_fd);
		else
		{
			last_pid = pid;
			parent_process(shell, pipefd, &prev_fd);
		}
		shell->cmds = shell->cmds->next;
	}
	while ((waited_pid = wait(&status)) > 0)
	{
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
			{
				shell->exit_code = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				shell->exit_code = 128 + WTERMSIG(status);
				if (shell->exit_code == 130)
					write(STDOUT_FILENO, "\n", 1);
				else if (shell->exit_code == 130)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
			}
		}
		else
		{
			if (WIFSIGNALED(status))
			{
				tmp2 = WTERMSIG(status);
				if (tmp2 == 2)
					write(STDOUT_FILENO, "\n", 1);
				else if (tmp2 == 3)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
			}
		}
	}
	setup_signals_interactive();
	free_cmd_list(tmp);
}
