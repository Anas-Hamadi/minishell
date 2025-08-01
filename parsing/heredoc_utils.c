/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:29:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:28:23 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

static char	*build_filename_prefix(char **fn, size_t *fn_size, size_t *fn_len)
{
	char	*pid_str;

	if (!safe_strcat_realloc(fn, fn_size, fn_len, "/tmp/.heredoc_"))
		return (NULL);
	pid_str = ft_itoa_simple((int)getpid());
	if (!pid_str)
	{
		free(*fn);
		return (NULL);
	}
	if (!safe_strcat_realloc(fn, fn_size, fn_len, pid_str))
	{
		free(pid_str);
		return (NULL);
	}
	free(pid_str);
	return (*fn);
}

static char	*build_filename_suffix(char **fn, size_t *fn_size, size_t *fn_len,
		int *hdoc_count)
{
	char	*count_str;

	if (!safe_charcat_realloc(fn, fn_size, fn_len, '_'))
		return (NULL);
	count_str = ft_itoa_simple((*hdoc_count)++);
	if (!count_str)
	{
		free(*fn);
		return (NULL);
	}
	if (!safe_strcat_realloc(fn, fn_size, fn_len, count_str))
	{
		free(count_str);
		return (NULL);
	}
	free(count_str);
	return (*fn);
}

char	*heredoc_generate_filename(struct s_shell *shell, int *hdoc_count)
{
	char	*fn;
	size_t	fn_size;
	size_t	fn_len;

	fn = malloc(64);
	fn_size = 64;
	fn_len = 0;
	if (!fn)
		return (NULL);
	fn[0] = '\0';
	if (!build_filename_prefix(&fn, &fn_size, &fn_len))
		return (NULL);
	if (!build_filename_suffix(&fn, &fn_size, &fn_len, hdoc_count))
		return (NULL);
	add_temp_file(shell, fn);
	return (fn);
}
