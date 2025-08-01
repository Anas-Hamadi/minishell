/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:12:40 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:29:54 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

static int	handle_quote_in_word(struct s_shell *shell, char **cmd,
		t_word_buffer *wb, bool in_del)
{
	char	quote;
	char	*quoted;

	quoted = handle_quote_block(shell, cmd, &quote, in_del);
	if (!quoted)
		return (0);
	if (!safe_strcat_realloc(wb->buffer, wb->buffer_size, wb->buffer_len,
			quoted))
	{
		free(quoted);
		return (0);
	}
	*(wb->found_quotes) = true;
	free(quoted);
	return (1);
}

static int	handle_dollar_in_word(struct s_shell *shell, char **cmd,
		t_word_buffer *wb)
{
	char	*expanded;

	expanded = expand_variable(shell, cmd);
	if (!expanded)
		return (0);
	if (!safe_strcat_realloc(wb->buffer, wb->buffer_size, wb->buffer_len,
			expanded))
	{
		free(expanded);
		return (0);
	}
	free(expanded);
	return (1);
}

static char	*init_word_buffer(void)
{
	char	*buffer;

	buffer = malloc(256);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	return (buffer);
}

static int	process_word_char(struct s_shell *shell, char **cmd,
		t_word_buffer *wb, bool in_del)
{
	if (**cmd == '\'' || **cmd == '"')
	{
		if (!handle_quote_in_word(shell, cmd, wb, in_del))
			return (0);
	}
	else if (**cmd == '$' && !in_del)
	{
		if (!handle_dollar_in_word(shell, cmd, wb))
			return (0);
	}
	else
	{
		if (!safe_charcat_realloc(wb->buffer, wb->buffer_size, wb->buffer_len,
				**cmd))
			return (0);
		(*cmd)++;
	}
	return (1);
}

char	*handle_word(struct s_shell *shell, char **cmd, bool in_del,
		bool *expand_in_hd)
{
	char			*buffer;
	size_t			buffer_size;
	size_t			buffer_len;
	bool			found_quotes;
	t_word_buffer	wb;

	buffer_len = 0;
	found_quotes = false;
	buffer_size = 256;
	buffer = init_word_buffer();
	if (!buffer)
		return (NULL);
	wb = (t_word_buffer){&buffer, &buffer_size, &buffer_len, &found_quotes};
	while (**cmd && !is_metachar(**cmd))
	{
		if (!process_word_char(shell, cmd, &wb, in_del))
		{
			free(buffer);
			return (NULL);
		}
	}
	if (expand_in_hd && found_quotes)
		*expand_in_hd = false;
	return (buffer);
}
