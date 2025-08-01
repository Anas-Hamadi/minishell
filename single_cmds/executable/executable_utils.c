/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:28:22 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 22:44:36 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	array = malloc(sizeof(char *) * (i + 1));
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

static char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full_patch;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_patch = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_patch, X_OK) == 0)
			return (ft_free(paths), full_patch);
		free(full_patch);
		i++;
	}
	return (ft_free(paths), NULL);
}

char	*find_cmd_path(char *cmd, t_list *t_envp)
{
	char	**paths;
	char	*path_value;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	path_value = get_path_value(t_envp);
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	free(path_value);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
