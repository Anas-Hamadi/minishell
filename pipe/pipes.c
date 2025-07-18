#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

void handle_pipes(t_shell *shell)
{
	int pid;
	int pipefd[2];
	int prev_fd = -1;
	t_cmdnode *tmp;

	tmp = shell->cmds;
	while (shell->cmds)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			if (prev_fd != -1) // if we are on the second token we need to take input from prev_fd
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if (shell->cmds->next) // if not then the output will be in the write side of the pipe (pipefd[1])
			{
				dup2(pipefd[1], 1);
				close(pipefd[1]);
			}
			handle_redirs(shell);
			if (!check_builtin(shell))
				check_exec(shell);
			exit(127);
		}
		else // this is the parent
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (shell->cmds->next)
			{
				close(pipefd[1]);
				prev_fd = pipefd[0];
			}
		}
		shell->cmds = shell->cmds->next;
	}
	while (wait(NULL) > 0)
		;
	free_cmd_list(tmp);
}
