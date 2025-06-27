#include "../minishell.h"

// // ls | grep .c | wc -l
// // svae the original in and output (redirections)
//
// void	handle_pipes(t_shell *shell)
// {
// 	int		i;
// 	int		pid;
// 	int		fds[2];
// 	int		prev_fd;
// 	char	**current_cmd;
// 	char	**env_arr;
// 	char	*cmd_path;
//
// 	i = 0;
// 	pid = 0;
// 	prev_fd = -1;
// 	env_arr = list_to_array(shell->t_envp);
// 	while (shell->s_input[i])
// 	{
// 		current_cmd = ft_split(shell->s_input[i], ' ');// the input was already splitted by '|' now we get rid of ' '
// 		if (shell->s_input[i + 1])
// 			pipe(fds);
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			if (prev_fd != -1) // when we are on the second cmd prev_fd has changed so we need to change the current input to be the prev_fd
// 			{
// 				dup2(prev_fd, 0);
// 				close(prev_fd);
// 			}
// 			if (shell->s_input[i + 1])
// 			{
// 				close(fds[0]);
// 				dup2(fds[1], 1);
// 				close(fds[1]);
// 			}
// 			cmd_path = find_cmd_path(current_cmd[0], shell->t_envp);
// 			if (!cmd_path)
// 			{
// 				perror("command not found");
// 				exit (127);
// 			}
// 			execve(cmd_path, current_cmd, env_arr);
// 			free(cmd_path);
// 		}
// 		else
// 		{
// 			if (prev_fd != -1) // we done reading from the prev_fd which is the output of the prev cmd so we close it .
// 				close(prev_fd);
// 			if (shell->s_input[i + 1])
// 			{
// 				close (fds[1]); //close write-end not needed in parent 
// 				prev_fd = fds[0];
// 			}
// 		}
// 		ft_free(current_cmd);
// 		i++;
// 	}
// 	ft_free(env_arr);
// 	while (wait(NULL) != -1)
// 		;
// }
//

void	handle_pipes(t_cmdnode *cmd_list)
{
	int pid;
	while (cmd_list->next)
	{
		pid = fork;
		if (pid == 0)
		{
			
		}
	}
}
