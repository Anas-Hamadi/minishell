/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:27:22 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 15:29:06 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exit_status(int pid, t_shell *shell)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_code = 128 + WTERMSIG(status);
		if (shell->exit_code == 130)
			write(STDOUT_FILENO, "\n", 1);
		else if (shell->exit_code == 131)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
	}
}

static void	execute_child_process(char *cmd_path, char **s_input,
		char **env_array)
{
	struct stat	st;

	if (access(cmd_path, F_OK) != 0)
	{
		ft_exec_error(env_array, s_input[0], 127);
	}
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(RED "minishell: " RESET, 2);
		ft_putstr_fd(s_input[0], 2);
		ft_putendl_fd(": Is a directory" RESET, 2);
		ft_free(env_array);
		exit(126);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		ft_exec_error(env_array, s_input[0], 126);
	}
	else
	{
		execve(cmd_path, s_input, env_array);
		perror("execve");
		ft_free(env_array);
		exit(127);
	}
}

void	execute_cmd(char *cmd_path, char **s_input, t_list *t_envp,
		t_shell *shell)
{
	int			pid;
	char		**env_array;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror("fork"));
	if (pid == 0)
	{
		setup_signals_child();
		env_array = list_to_array(t_envp);
		execute_child_process(cmd_path, s_input, env_array);
	}
	else
	{
		exit_status(pid, shell);
		setup_signals_interactive();
	}
}

static void	handle_empty_cmd(t_shell *shell)
{
	shell->exit_code = handle_empty_command(shell);
}

void	check_exec(t_shell *shell)
{
	char	*full_path;

	if (!shell->cmds->argv)
		return ;
	if (!shell->cmds->argv[0] || shell->cmds->argv[0][0] == '\0')
	{
		handle_empty_cmd(shell);
		return ;
	}
	if (ft_strchr(shell->cmds->argv[0], '/'))
	{
		execute_cmd(shell->cmds->argv[0], shell->cmds->argv, shell->envp,
			shell);
		return ;
	}
	full_path = find_cmd_path(shell->cmds->argv[0], shell->envp);
	if (!full_path)
	{
		handle_cmd_not_found(shell);
		return ;
	}
	else
		execute_cmd(full_path, shell->cmds->argv, shell->envp, shell);
	free(full_path);
}
