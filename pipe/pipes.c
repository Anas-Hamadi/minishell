/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 21:53:51 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/03 09:58:31 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_absolute_path(t_shell *shell, char **env_array)
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

static void	exec_cmd_child(t_shell *shell)
{
	char	*full_path;
	char	**env_array;

	if (!shell->cmds->argv)
		exit(0);
	if (!shell->cmds->argv[0] || shell->cmds->argv[0][0] == '\0')
		exit(handle_empty_command(shell));
	env_array = list_to_array(shell->envp);
	if (ft_strchr(shell->cmds->argv[0], '/'))
		exec_absolute_path(shell, env_array);
	full_path = find_cmd_path(shell->cmds->argv[0], shell->envp);
	if (!full_path)
	{
		ft_putstr_fd(RED "minishell: command not found: " RESET, 2);
		ft_putendl_fd(shell->cmds->argv[0], 2);
		ft_free(env_array);
		exit(127);
	}
	else
		exec_with_path(shell, full_path, env_array);
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
	t_pipe_data	data;

	data.prev_fd = -1;
	data.tmp = shell->cmds;
	data.last_pid = -1;
	signal(SIGINT, SIG_IGN);
	while (shell->cmds)
	{
		if (shell->cmds->next)
			pipe(data.pipefd);
		data.pid = fork();
		if (data.pid == 0)
			child_process(shell, data.pipefd, data.prev_fd);
		else
		{
			data.last_pid = data.pid;
			parent_process(shell, data.pipefd, &data.prev_fd);
		}
		shell->cmds = shell->cmds->next;
	}
	wait_for_children(shell, data.last_pid);
	setup_signals_interactive();
	free_cmd_list(data.tmp);
}
