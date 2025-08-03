/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:33:57 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 22:36:48 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t	g_signal_num = 0;

void	init_shell(t_shell *shell, char **envp)
{
	shell->envp_list = envp_to_list(envp);
	init_default_env(&shell->envp_list);
	shell->envp = shell->envp_list;
	shell->cmds = NULL;
	shell->input = NULL;
	shell->exit_code = 0;
	shell->temp_files = NULL;
	shell->temp_count = 0;
	shell->temp_capacity = 0;
	shell->saved_in = -1;
	shell->saved_out = -1;
	shell->stdio_saved = 0;
}

static int	save_stdio(t_shell *shell)
{
	shell->saved_in = dup(0);
	shell->saved_out = dup(1);
	if (shell->saved_in < 0 || shell->saved_out < 0)
	{
		if (shell->saved_in >= 0)
			close(shell->saved_in);
		if (shell->saved_out >= 0)
			close(shell->saved_out);
		shell->stdio_saved = 0;
		return (0);
	}
	shell->stdio_saved = 1;
	return (1);
}

void	start(t_shell *shell)
{
	if (!save_stdio(shell))
		return ;
	shell->cmds = parse_command_line(shell, shell->input);
	if (shell->cmds == NULL)
	{
		handle_parse_error(shell);
		restore_stdio(shell->saved_in, shell->saved_out);
		shell->stdio_saved = 0;
		return ;
	}
	if (shell->cmds->next)
		handle_pipes(shell);
	else
		handle_single_cmd(shell);
	free_cmd_list(shell->cmds);
	shell->cmds = NULL;
	free(shell->input);
	shell->input = NULL;
	restore_stdio(shell->saved_in, shell->saved_out);
	shell->stdio_saved = 0;
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	setup_signals_interactive();
	init_shell(&shell, envp);
	while (true)
	{
		shell.input = readline(YELLOW "minishell$ " RESET);
		if (handle_input(&shell))
			check_signals_after_execution(&shell);
	}
	rl_clear_history();
	cleanup_temp_files(&shell);
	free_shell(&shell);
}
