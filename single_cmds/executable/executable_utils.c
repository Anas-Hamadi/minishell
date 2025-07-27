/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:28:22 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 15:29:27 by molamham         ###   ########.fr       */
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

char	*find_cmd_path(char *cmd, t_list *t_envp)
{
	int		i;
	char	**paths;
	char	*full_patch;
	char	*tmp;
	char	*path_value;

	path_value = get_path_value(t_envp);
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	free(path_value);
	if (!paths)
		return (NULL);
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
