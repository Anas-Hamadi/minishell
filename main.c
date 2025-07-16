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

void	handle_redirs(t_cmdnode *cmd_list)
{
	int fd;
	t_redir *redirs = cmd_list->redirs;
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

void	start(char *input, t_list *envp)
{
	int			saved_in;
	int			saved_out;
	t_cmdnode	*cmd_list;
	t_cmdnode	*cur;

//	int			exit_status;

	saved_in = dup(0);
	saved_out = dup(1);
	cmd_list = parse_command_line(input);
	// printf("%s\n%s\n%s\n", cmd_list->argv[0], cmd_list->argv[1], cmd_list->argv[2]);
	cur = cmd_list;
	while (cur)
	{
		cur->envp = envp;
		cur = cur->next;
	}
	if (cmd_list->next)
		handle_pipes(cmd_list);
	else
		handle_single_cmd(cmd_list);
	dup2(saved_in, 0);
	dup2(saved_out, 1);

	// WARN: close might be wrong (dup() sys call no need to close)
	// close(saved_out);
	// close(saved_in);


	free_cmd_list(cmd_list);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char *input;
	t_list	*t_envp;

	t_envp = envp_to_list(envp);





	while (true) // infinite loop to read and execute commands
	{
		input = readline(YELLOW "minishell$ " RESET);

		if (!input) // ctrl+d end of file
			break ;

		if (*input) 
			add_history(input);

		else // skip empty commands
		{
			free(input);
			continue ;
		}
		start(input, t_envp);
		free(input);
	}
}

