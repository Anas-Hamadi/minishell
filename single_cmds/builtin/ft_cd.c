/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:40:47 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 22:43:11 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_target_path(char **argv)
{
	char	*home;

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

void	update_cd_env(t_list **envp, char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("getcwd");
		free(oldpwd);
		return ;
	}
	update_env(envp, "OLDPWD", oldpwd);
	update_env(envp, "PWD", newpwd);
	free(newpwd);
}

static int	change_directory(char *path, char **oldpwd)
{
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		free(*oldpwd);
		return (1);
	}
	return (0);
}

void	ft_cd(t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	char	**argv;

	argv = shell->cmds->argv;
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_code = 1;
		return ;
	}
	path = get_target_path(argv);
	if (!path)
	{
		shell->exit_code = 1;
		return ;
	}
	if (change_directory(path, &oldpwd) != 0)
	{
		shell->exit_code = 1;
		return ;
	}
	update_cd_env(&shell->envp, oldpwd);
	free(oldpwd);
	shell->exit_code = 0;
}
