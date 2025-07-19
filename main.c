/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:33:57 by molamham          #+#    #+#             */
/*   Updated: 2025/07/19 16:35:32 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parse.h"
#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_shell(t_shell *shell, char **envp)
{
	shell->envp_list = envp_to_list(envp);
	shell->envp = shell->envp_list;
	shell->cmds = NULL;
	shell->input = NULL;
	shell->exit_code = 0;
}

void sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);		// Print a newline on Ctrl+C
		rl_replace_line("", 0); // Clear the current line in readline
		rl_on_new_line();		// Move to a new line
		rl_redisplay();			// Redisplay the prompt
	}
}

void start(t_shell *shell)
{
	int saved_in;
	int saved_out;

	saved_in = dup(0);
	saved_out = dup(1);
	shell->cmds = parse_command_line(shell->input);
	if (shell->cmds == NULL)
	{
		ft_putstr_fd(RED "minishell: parse error\n" RESET, 2);
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

int main(int ac, char **av, char **envp)
{
	t_shell shell;

	(void)ac;
	(void)av;
	signal(SIGINT, sigint_handler); // Handle Ctrl+C
	init_shell(&shell, envp);
	while (true) // infinite loop to read and execute commands
	{
		shell.input = readline(YELLOW "minishell$ " RESET);
		if (!shell.input) // ctrl+d end of file
			break;
		if (*shell.input)
			add_history(shell.input);
		else // skip empty commands
		{
			free(shell.input);
			continue;
		}
		start(&shell);
	}
	free_shell(&shell); // Free all allocated memory
}
