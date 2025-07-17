#include "minishell.h"
#include "parsing/parse.h"
#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// will be used later 
// void	handle_single_redir(char *filename, int flags, int std_fd)
// {
// 	int fd = open(filename, flags, 0644);
// 	if (fd < 0)
// 		perror("open");
// 	else
// 	{
// 		dup2(fd, std_fd);
// 		close(fd);
// 	}
// }

void	handle_redirs(t_shell *shell)
{
	int fd;
	t_redir *redirs = shell->cmds->redirs;
	while (redirs)
	{
		if (redirs->type == R_IN)
		{
			fd = open(redirs->filename, O_RDONLY);
			if (fd < 0)
				perror("open");
			else
			{
				dup2(fd, 0);
				close(fd);
			}
		}
		if (redirs->type == R_OUT)
		{
			fd = open(redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				perror("open");
			else
			{
				dup2(fd, 1);
				close(fd);
			}
		}
		if (redirs->type == R_APPEND)
		{
			fd = open(redirs->filename, O_WRONLY | O_CREAT | O_APPEND , 0644);
			if (fd < 0)
				perror("open");
			else
			{
				dup2(fd, 1);
				close(fd);
			}
		}
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

	// WARN: close might be wrong (dup() sys call no need to close)
	// close(saved_out);
	// close(saved_in);


	//free_cmd_list(cmd_list);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_shell shell;

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

