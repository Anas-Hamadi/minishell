#include "../../minishell.h"
#include <unistd.h>

char	**list_to_array(t_list *t_envp)
{
	int		i;
	char	**array;
t_list	*tmp;

	i = 0;
	tmp = t_envp;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	array = malloc (sizeof(char *) * (i + 1));
	if (!array)
		return (NULL);
	i = 0;
	tmp = t_envp;
	while (tmp)
	{
		array[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

char	*get_path_value(t_list *t_envp)
{
	t_list	*tmp;
	char	*path;

	tmp = t_envp;
	while (tmp)
	{
		if (ft_strncmp(tmp->content, "PATH=", 5) == 0)
		{
			path = ft_strdup(tmp->content + 5);
			return (path);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_list *t_envp) //*find_cmd_path(t_shell *shell)
{
	int		i;
	char	**paths;
	char	*full_patch;

	paths = ft_split(get_path_value(t_envp), ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_patch = ft_strjoin(paths[i], "/"); // add '/' to the path to try and create a full cmd path and check if its valid.
		full_patch = ft_strjoin(full_patch, cmd);
		if (access(full_patch, X_OK) == 0)  // access : system call that checks your permissions for a file.
		{
			ft_free(paths);
			return (full_patch);
		}
		free(full_patch);
		i++;
	}
	ft_free(paths);
	return (NULL);
}

void	execute_cmd(char *cmd_path, char **s_input, t_list *t_envp) //execute_cmd(t_shell *shell)
{
	int		status;
	int		pid;
	char	**env_array;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		env_array = list_to_array(t_envp); //env_array = shell->s_input;
		if (access(cmd_path, X_OK) == 0) //(access(shell->s_input[0], X_OK) == 0)
		{
			execve(cmd_path, s_input, env_array);
			perror("execve");
		}
		else
			printf("minishell: %s: Permission denied or file not found\n", s_input[0]);
		ft_free(env_array);
		exit(127);//Code 127 is like the classic “command not found” exit status in Unix/Linux shells.
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
				status = WTERMSIG(status);
		// if signaled check to change the exit status to 130 or 131
	}
}

void	check_exec(t_shell *shell)
{
	char *full_path;

	if (!shell->cmds->argv)
		return;
	if (ft_strchr(shell->cmds->argv[0], '/'))
	{
		execute_cmd(shell->cmds->argv[0], shell->cmds->argv, shell->envp);
		return ;
	}
	full_path = find_cmd_path(shell->cmds->argv[0], shell->envp);
	if (!full_path)
	{
		ft_putstr(RED "minishell: command not found: " RESET, 2);
		ft_putendl_fd(shell->cmds->argv[0], 2);
		return ;
	}
	else
		execute_cmd(full_path, shell->cmds->argv, shell->envp);
	free(full_path);
}
