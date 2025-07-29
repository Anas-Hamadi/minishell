/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:44:31 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:56:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void exec_cmd_child(t_shell *shell) {
    char *full_path;
    char **env_array;

    if (!shell->cmds->argv)
	exit(0);

    env_array = list_to_array(shell->envp);

    if (ft_strchr(shell->cmds->argv[0], '/')) {
	// Direct path
	if (access(shell->cmds->argv[0], X_OK) == 0) {
	    execve(shell->cmds->argv[0], shell->cmds->argv, env_array);
	    perror("execve");
	} else {
	    ft_putstr_fd(RED "minishell: " RESET, 2);
	    ft_putstr_fd(shell->cmds->argv[0], 2);
	    ft_putendl_fd(": Permission denied or file not found" RESET, 2);
	}
	ft_free(env_array);
	exit(127);
    }

    full_path = find_cmd_path(shell->cmds->argv[0], shell->envp);
    if (!full_path) {
	ft_putstr_fd(RED "minishell: command not found: " RESET, 2);
	ft_putendl_fd(shell->cmds->argv[0], 2);
	ft_free(env_array);
	exit(127);
    } else {
	if (access(full_path, X_OK) == 0) {
	    execve(full_path, shell->cmds->argv, env_array);
	    perror("execve");
	} else {
	    ft_putstr_fd(RED "minishell: " RESET, 2);
	    ft_putstr_fd(shell->cmds->argv[0], 2);
	    ft_putendl_fd(": Permission denied or file not found" RESET, 2);
	}
    }
    ft_free(env_array);
    free(full_path);
    exit(127);
}

static void child_process(t_shell *shell, int pipefd[2], int prev_fd) {
    setup_signals_child();  // Setup proper signal handling for child
    if (prev_fd != -1) {
	dup2(prev_fd, 0);
	close(prev_fd);
    }
    if (shell->cmds->next) {
	dup2(pipefd[1], 1);
	close(pipefd[1]);
	close(pipefd[0]);  // Close read end in child
    }
    handle_redirs(shell);
    if (check_builtin(shell))
	exit(shell->exit_code);  // Exit with builtin's exit code
    else
	exec_cmd_child(shell);  // This will exec or exit, never returns
}

static void parent_process(t_shell *shell, int pipefd[2], int *prev_fd) {
    if (*prev_fd != -1)
	close(*prev_fd);
    if (shell->cmds->next) {
	close(pipefd[1]);      // Close write end in parent
	*prev_fd = pipefd[0];  // Keep read end for next command
    }
}

void handle_pipes(t_shell *shell) {
    int pid;
    int pipefd[2];
    int prev_fd;
    t_cmdnode *tmp;
    int last_pid;
    int status;
    int waited_pid;

    prev_fd = -1;
    tmp = shell->cmds;
    last_pid = -1;
    while (shell->cmds) {
	// Only create pipe if there's a next command
	if (shell->cmds->next)
	    pipe(pipefd);

	pid = fork();
	if (pid == 0)
	    child_process(shell, pipefd, prev_fd);
	else {
	    last_pid = pid;  // Keep track of the last command in pipeline
	    parent_process(shell, pipefd, &prev_fd);
	}
	shell->cmds = shell->cmds->next;
    }

    // Wait for all children and capture exit code of last command
    while ((waited_pid = wait(&status)) > 0) {
	if (waited_pid == last_pid) {
	    if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	    else if (WIFSIGNALED(status))
		shell->exit_code = 128 + WTERMSIG(status);
	}
    }
    free_cmd_list(tmp);
}
