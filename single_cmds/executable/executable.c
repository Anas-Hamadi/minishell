/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:27:22 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:56:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_permission_denied(char **env_array, char *s_input) {
    ft_putstr_fd(RED "minishell: " RESET, 2);
    ft_putstr_fd(s_input, 2);
    ft_putendl_fd(": Permission denied or file not found" RESET, 2);
    ft_free(env_array);
    exit(127);
}

void exit_status(int pid, t_shell *shell) {
    int status;

    status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
	shell->exit_code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status)) {
	/* Signal exit codes follow the convention: 128 + signal_number */
	shell->exit_code = 128 + WTERMSIG(status);
    }
}

void execute_cmd(char *cmd_path, char **s_input, t_list *t_envp,
                 t_shell *shell) {
    int pid;
    char **env_array;

    pid = fork();
    if (pid < 0)
	return (perror("fork"));
    if (pid == 0) {
	// Setup default signal handling for child process
	setup_signals_child();

	env_array = list_to_array(t_envp);
	if (access(cmd_path, X_OK) == 0) {
	    execve(cmd_path, s_input, env_array);
	    perror("execve");
	} else
	    ft_permission_denied(env_array, s_input[0]);
	ft_free(env_array);
	exit(127);
    } else
	exit_status(pid, shell);
}

void check_exec(t_shell *shell) {
    char *full_path;

    if (!shell->cmds->argv)
	return;
    if (ft_strchr(shell->cmds->argv[0], '/')) {
	execute_cmd(shell->cmds->argv[0], shell->cmds->argv, shell->envp,
	            shell);
	return;
    }
    full_path = find_cmd_path(shell->cmds->argv[0], shell->envp);
    if (!full_path) {
	ft_putstr_fd(RED "minishell: command not found: " RESET, 2);
	ft_putendl_fd(shell->cmds->argv[0], 2);
	shell->exit_code = 127;
	return;
    } else {
	execute_cmd(full_path, shell->cmds->argv, shell->envp, shell);
    }
    free(full_path);
}
