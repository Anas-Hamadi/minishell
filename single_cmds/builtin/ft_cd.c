/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:40:47 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 20:50:19 by ahamadi          ###   ########.fr       */
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

static void	handle_getcwd_failure(t_list **envp, char *oldpwd)
{
	char	*home;

	home = find_env_value(*envp, "HOME");
	if (home && chdir(home) == 0)
	{
		char	*newpwd;

		newpwd = getcwd(NULL, 0);
		if (!newpwd)
		{
			update_env(envp, "PWD", home);
			if (oldpwd)
				update_env(envp, "OLDPWD", oldpwd);
			return ;
		}
		if (oldpwd)
			update_env(envp, "OLDPWD", oldpwd);
		update_env(envp, "PWD", newpwd);
		free(newpwd);
	}
	else
	{
		ft_putstr_fd("cd: current directory removed and cannot access home\n", 2);
		if (oldpwd)
			free(oldpwd);
	}
}

void	update_cd_env(t_list **envp, char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		handle_getcwd_failure(envp, oldpwd);
		return ;
	}
	if (oldpwd)
		update_env(envp, "OLDPWD", oldpwd);
	update_env(envp, "PWD", newpwd);
	free(newpwd);
}

static int	change_directory(char *path, char **oldpwd)
{
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		ft_putstr_fd("cd: current directory no longer exists\n", 2);
		*oldpwd = NULL;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		if (*oldpwd)
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
