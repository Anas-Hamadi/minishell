/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_to_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:04:43 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 22:37:56 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env(t_list **envp, char *key, char *value)
{
	char	*new_str;
	char	*tmp_str;
	t_list	*tmp;

	tmp = *envp;
	tmp_str = ft_strjoin(key, "=");
	new_str = ft_strjoin(tmp_str, value);
	free(tmp_str);
	while (tmp)
	{
		if (!ft_strncmp((char *)tmp->content, key, ft_strlen(key))
			&& ((char *)tmp->content)[ft_strlen(key)] == '=')
		{
			free(tmp->content);
			tmp->content = new_str;
			return ;
		}
		tmp = tmp->next;
	}
	ft_lstadd_back(envp, ft_lstnew(ft_strdup(new_str)));
	free(new_str);
}

t_list	*envp_to_list(char **envp)
{
	int		i;
	t_list	*t_envp;
	t_list	*new;
	char	*dup;

	i = 0;
	t_envp = NULL;
	while (envp[i])
	{
		dup = ft_strdup(envp[i]);
		if (!dup)
			return (ft_lstclear(&t_envp, free), NULL);
		new = ft_lstnew(dup);
		if (!new)
		{
			free(dup);
			return (ft_lstclear(&t_envp, free), NULL);
		}
		ft_lstadd_back(&t_envp, new);
		i++;
	}
	return (t_envp);
}

static void	init_pwd(t_list **envp)
{
	char	*pwd;

	if (!find_env_value(*envp, "PWD"))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			update_env(envp, "PWD", pwd);
			free(pwd);
		}
	}
}

static void	init_shlvl(t_list **envp)
{
	char	*shlvl_str;
	int		shlvl;

	if (!find_env_value(*envp, "SHLVL"))
		update_env(envp, "SHLVL", "1");
	else
	{
		shlvl_str = find_env_value(*envp, "SHLVL");
		shlvl = ft_atoi(shlvl_str) + 1;
		shlvl_str = ft_itoa_simple(shlvl);
		if (shlvl_str)
		{
			update_env(envp, "SHLVL", shlvl_str);
			free(shlvl_str);
		}
	}
}

void	init_default_env(t_list **envp)
{
	init_pwd(envp);
	init_shlvl(envp);
	if (!find_env_value(*envp, "_"))
		update_env(envp, "_", "/usr/bin/env");
}
