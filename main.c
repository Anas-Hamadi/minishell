/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:33:57 by molamham          #+#    #+#             */
/*   Updated: 2025/07/18 12:33:59 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parse.h"
#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	handle_single_redir(char *filename, int flags, int std_fd)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
		perror("open");
	else
	{
		dup2(fd, std_fd);
		close(fd);
	}
}

void	handle_redirs(t_shell *shell)
{
	t_redir	*redirs;

	redirs = shell->cmds->redirs;
	while (redirs)
	{
		if (redirs->type == R_IN)
			handle_single_redir(redirs->filename, O_RDONLY, 0);
		else if (redirs->type == R_OUT)
			handle_single_redir(redirs->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (redirs->type == R_APPEND)
			handle_single_redir(redirs->filename,
				O_WRONLY | O_CREAT | O_APPEND, 1);
		redirs = redirs->next;
	}
}

void	start(t_shell *shell)
{
	int			saved_in;
	int			saved_out;

	saved_in = dup(0);
	saved_out = dup(1);
	shell->cmds = parse_command_line(shell->input);
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

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.envp = envp_to_list(envp);
	while (true) // infinite loop to read and execute commands
	{
		shell.input = readline(YELLOW "minishell$ " RESET);
		if (!shell.input) // ctrl+d end of file
			break ;
		if (*shell.input)
			add_history(shell.input);
		else // skip empty commands
		{
			free(shell.input);
			continue ;
		}
		start(&shell);
		free(shell.input);
	}
}
