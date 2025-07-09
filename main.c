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
	(void)envp;
	int			saved_in;
	int			saved_out;
	t_cmdnode	*cmd_list;
	t_cmdnode	*cur;

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
	close(saved_out);
	close(saved_in);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char *input;
	t_list	*t_envp;

	t_envp = envp_to_list(envp);
	while (true)
	{
		input = readline(YELLOW "minishell$ " RESET);
		if (!input)
			break ;
		if (*input) // skip empty commands
			add_history(input);
		else
		{
			free(input);
			continue ;
		}
		start(input, t_envp);
		free(input);
	}
}




















































// int	main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// 	t_shell	shell;
//
// 	shell.t_envp = envp_to_list(envp);
// 	while (1)
// 	{
// 		shell.input = readline("\033[34mMinishel$ \033[0m");
// 		if (shell.input)
// 		{
// 			if (ft_strchr(shell.input, '|'))
// 			{
// 				shell.s_input = ft_split(shell.input, '|');
// 				handle_pipes(&shell);
// 			}
// 			else 
// 			{
// 				shell.s_input = ft_split(shell.input, ' ');
// 				if (!check_builtin(&shell))
// 					check_exec(&shell);
// 			}
// 			add_history(shell.input);
// 		}
// 		if (shell.s_input)
// 			ft_free(shell.s_input);
// 	}
// 	ft_free(shell.s_input);
// 	free(shell.input);
// }
/*==========================================================================*/

