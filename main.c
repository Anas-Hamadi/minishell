/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:33:57 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:56:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "minishell.h"

void init_shell(t_shell *shell, char **envp) {
    shell->envp_list = envp_to_list(envp);
    shell->envp = shell->envp_list;
    shell->cmds = NULL;
    shell->input = NULL;
    shell->exit_code = 0;
    shell->temp_files = NULL;
    shell->temp_count = 0;
    shell->temp_capacity = 0;
}

void start(t_shell *shell) {
    int saved_in;
    int saved_out;

    saved_in = dup(0);
    saved_out = dup(1);
    shell->cmds = parse_command_line(shell, shell->input);
    if (shell->cmds == NULL) {
	// Check if parsing was interrupted by a signal
	if (g_signal_num == SIGINT) {
	    shell->exit_code = 130;
	} else if (g_signal_num == SIGQUIT) {
	    shell->exit_code = 131;
	} else {
	    ft_putstr_fd(RED "minishell: parse error\n" RESET, 2);
	    shell->exit_code = 2;  // Parse error exit code
	}
	dup2(saved_in, 0);
	dup2(saved_out, 1);
	close(saved_in);
	close(saved_out);
	return;
    }
    if (shell->cmds->next)
	handle_pipes(shell);
    else
	handle_single_cmd(shell);
    free_cmd_list(shell->cmds);
    shell->cmds = NULL;
    dup2(saved_in, 0);
    dup2(saved_out, 1);
    close(saved_in);
    close(saved_out);
}

int main(int ac, char **av, char **envp) {
    t_shell shell;

    (void)ac;
    (void)av;

    // Setup interactive mode signals
    setup_signals_interactive();

    init_shell(&shell, envp);
    while (true) {
	// Check for signals before prompting
	// check_signal_interactive();

	shell.input = readline(YELLOW "minishell$ " RESET);
	if (!shell.input)  // Ctrl-D pressed
	    break;

	if (*shell.input)
	    add_history(shell.input);
	else {
	    free(shell.input);
	    continue;
	}

	// Reset signal state before processing command
	g_signal_num = 0;
	start(&shell);

	// Check for signals after command execution
	if (g_signal_num == SIGINT) {
	    shell.exit_code = 130;  // Standard exit code for SIGINT
	    g_signal_num = 0;       // Reset signal
	} else if (g_signal_num == SIGQUIT) {
	    shell.exit_code = 131;  // Standard exit code for SIGQUIT
	    g_signal_num = 0;       // Reset signal
	}
    }
    rl_clear_history();
    cleanup_temp_files(&shell);  // Clean up all temp files before exit
    free_shell(&shell);
}
