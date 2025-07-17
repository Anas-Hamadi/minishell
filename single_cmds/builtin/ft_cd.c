#include "../../minishell.h"
#include <stdio.h>
#include <unistd.h>

// void	ft_cd(t_shell *shell)
// {
// 	char	*path;
//
// 	path = shell->s_input[1];
// 	if (!path)
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 		{
// 			printf("cd : HOME not set\n");
// 			return ;
// 		}
// 	}
// 	if (chdir(path) != 0)
// 		perror("cd");
// }

// void	ft_cd(t_cmdnode *cmd_list)
// {
// 	char	*path;
//
// 	path = cmd_list->argv[1];
// 	if (!path)
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 		{
// 			ft_putstr("cd : HOME not set\n", 2);
// 			return ;
// 		}
// 	}
// 	if (chdir(path) != 0)
// 		perror("cd");
// }

char *get_target_path(char **argv)
{
	char *home;

	if (argv[1])
		return (argv[1]);
	else
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putendl_fd("cd: home not set\n", 2);
			return (NULL);
		}
		return (home);
	}
}

void update_cd_env(t_list **envp, char *oldpwd)
{
	char *newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("getcwd");
		free(oldpwd);
		return;
	}

	update_env(envp, "OLDPWD", oldpwd);
	update_env(envp, "PWD", newpwd);
	free(newpwd);
}

void ft_cd(t_shell *shell)
{
	char *path;
	char *oldpwd;

	path = get_target_path(shell->cmds->argv);
	if (!path)
		return;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		return;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return;
	}
	update_cd_env(&shell->envp, oldpwd);
	free(oldpwd);
}
