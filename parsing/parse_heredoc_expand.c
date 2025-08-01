/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_expand.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:00:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:28:35 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

static char	*expand_hd_variable(struct s_shell *shell, char **cmd)
{
	char	*varname_start;
	char	saved;
	char	*env_value;

	if (**cmd == '?')
	{
		(*cmd)++;
		return (ft_itoa_simple(shell->exit_code));
	}
	varname_start = *cmd;
	while (**cmd && (ft_isalnum(**cmd) || **cmd == '_'))
		(*cmd)++;
	if (*cmd > varname_start)
	{
		saved = **cmd;
		**cmd = '\0';
		env_value = get_env_value(shell, varname_start);
		**cmd = saved;
		if (env_value)
			return (ft_strdup(env_value));
		else
			return (ft_strdup(""));
	}
	return (ft_strdup(""));
}

static int	process_hd_dollar(struct s_shell *shell, char **cmd,
		t_hd_buffer *hdb)
{
	char	*expanded;

	(*cmd)++;
	expanded = expand_hd_variable(shell, cmd);
	if (!expanded)
		return (0);
	if (!safe_strcat_realloc(hdb->buffer, hdb->buffer_size, hdb->buffer_len,
			expanded))
	{
		free(expanded);
		return (0);
	}
	free(expanded);
	return (1);
}

static int	process_hd_char(struct s_shell *shell, char **cmd, t_hd_buffer *hdb)
{
	if (**cmd == '$')
	{
		if (!process_hd_dollar(shell, cmd, hdb))
			return (0);
	}
	else
	{
		if (!safe_charcat_realloc(hdb->buffer, hdb->buffer_size,
				hdb->buffer_len, **cmd))
			return (0);
		(*cmd)++;
	}
	return (1);
}

char	*handle_hd_line(struct s_shell *shell, char **cmd)
{
	char		*buffer;
	size_t		buffer_size;
	size_t		buffer_len;
	t_hd_buffer	hdb;

	buffer_len = 0;
	buffer_size = 256;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	hdb = (t_hd_buffer){&buffer, &buffer_size, &buffer_len};
	while (**cmd)
	{
		if (!process_hd_char(shell, cmd, &hdb))
		{
			free(buffer);
			return (NULL);
		}
	}
	return (buffer);
}
