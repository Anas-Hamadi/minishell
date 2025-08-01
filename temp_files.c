/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:11:31 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 15:11:38 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_new_capacity(int current_capacity)
{
	if (current_capacity == 0)
		return (4);
	return (current_capacity * 2);
}

static char	**expand_temp_array(char **old_array, int old_cap, int new_cap)
{
	char	**new_array;
	int		i;

	new_array = malloc(sizeof(char *) * new_cap);
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < old_cap && old_array)
	{
		new_array[i] = old_array[i];
		i++;
	}
	while (i < new_cap)
		new_array[i++] = NULL;
	if (old_array)
		free(old_array);
	return (new_array);
}

void	add_temp_file(t_shell *shell, const char *filename)
{
	char	**new_array;
	int		new_capacity;

	if (!filename || !shell)
		return ;
	if (shell->temp_count >= shell->temp_capacity)
	{
		new_capacity = get_new_capacity(shell->temp_capacity);
		new_array = expand_temp_array(shell->temp_files, shell->temp_capacity,
				new_capacity);
		if (!new_array)
			return ;
		shell->temp_files = new_array;
		shell->temp_capacity = new_capacity;
	}
	shell->temp_files[shell->temp_count] = ft_strdup(filename);
	if (shell->temp_files[shell->temp_count])
		shell->temp_count++;
}

void	cleanup_temp_files(t_shell *shell)
{
	int	i;

	if (!shell || !shell->temp_files)
		return ;
	i = 0;
	while (i < shell->temp_count)
	{
		if (shell->temp_files[i])
		{
			unlink(shell->temp_files[i]);
			free(shell->temp_files[i]);
		}
		i++;
	}
	free(shell->temp_files);
	shell->temp_files = NULL;
	shell->temp_count = 0;
	shell->temp_capacity = 0;
}

void	remove_temp_file(t_shell *shell, const char *filename)
{
	int	i;

	if (!filename || !shell || !shell->temp_files)
		return ;
	i = 0;
	while (i < shell->temp_count)
	{
		if (shell->temp_files[i]
			&& ft_strcmp(shell->temp_files[i], filename) == 0)
		{
			unlink(shell->temp_files[i]);
			free(shell->temp_files[i]);
			shell->temp_files[i] = NULL;
			break ;
		}
		i++;
	}
}
