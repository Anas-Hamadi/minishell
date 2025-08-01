/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:34:01 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:29:11 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

char	*process_quoted_content(struct s_shell *shell, char **cmd, bool in_del)
{
	char	quote_context;

	return (handle_quote_block(shell, cmd, &quote_context, in_del));
}

char	*process_expansion(struct s_shell *shell, char **cmd, t_word_buffer *wb)
{
	char	*expanded;

	expanded = expand_variable(shell, cmd);
	if (!expanded)
		return (NULL);
	if (!safe_strcat_realloc(wb->buffer, wb->buffer_size, wb->buffer_len,
			expanded))
	{
		free(expanded);
		return (NULL);
	}
	free(expanded);
	return (*(wb->buffer));
}

int	handle_quote_char(struct s_shell *shell, char **cmd, bool in_del,
		t_word_buffer *wb)
{
	char	*quoted_content;

	quoted_content = process_quoted_content(shell, cmd, in_del);
	if (!quoted_content)
		return (0);
	if (!safe_strcat_realloc(wb->buffer, wb->buffer_size, wb->buffer_len,
			quoted_content))
	{
		free(quoted_content);
		return (0);
	}
	free(quoted_content);
	return (1);
}

int	handle_regular_char(char **cmd, t_word_buffer *wb)
{
	if (!safe_charcat_realloc(wb->buffer, wb->buffer_size, wb->buffer_len,
			**cmd))
		return (0);
	(*cmd)++;
	return (1);
}

int	process_word_char(struct s_shell *shell, char **cmd, bool in_del,
		t_word_buffer *wb)
{
	if (**cmd == '\'' || **cmd == '"')
		return (handle_quote_char(shell, cmd, in_del, wb));
	else if (**cmd == '$' && !in_del)
	{
		if (!process_expansion(shell, cmd, wb))
			return (0);
	}
	else
		return (handle_regular_char(cmd, wb));
	return (1);
}
