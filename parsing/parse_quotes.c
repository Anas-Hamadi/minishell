/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:41:20 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:28:53 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

static char	*handle_single_quotes(char **cmd, t_quote_buffer *qb)
{
	while (**cmd && **cmd != '\'')
	{
		if (!safe_charcat_realloc(qb->buffer, qb->buffer_size, qb->buffer_len,
				**cmd))
			return (NULL);
		(*cmd)++;
	}
	if (**cmd != '\'')
	{
		free(*(qb->buffer));
		return (NULL);
	}
	(*cmd)++;
	return (*(qb->buffer));
}

static char	*process_double_quote_char(struct s_shell *shell, char **cmd,
		t_quote_buffer *qb, bool in_del)
{
	char	*expanded;

	if (**cmd == '$' && !in_del)
	{
		expanded = expand_variable(shell, cmd);
		if (!expanded)
			return (NULL);
		if (!safe_strcat_realloc(qb->buffer, qb->buffer_size, qb->buffer_len,
				expanded))
		{
			free(expanded);
			return (NULL);
		}
		free(expanded);
	}
	else
	{
		if (!safe_charcat_realloc(qb->buffer, qb->buffer_size, qb->buffer_len,
				**cmd))
			return (NULL);
		(*cmd)++;
	}
	return (*(qb->buffer));
}

static char	*handle_double_quotes(struct s_shell *shell, char **cmd,
		t_quote_buffer *qb, bool in_del)
{
	while (**cmd && **cmd != '"')
	{
		if (!process_double_quote_char(shell, cmd, qb, in_del))
			return (NULL);
	}
	if (**cmd != '"')
	{
		free(*(qb->buffer));
		return (NULL);
	}
	(*cmd)++;
	return (*(qb->buffer));
}

char	*handle_quote_block(struct s_shell *shell, char **cmd,
		char *quote_context, bool in_del)
{
	char			quote;
	char			*buffer;
	size_t			buffer_size;
	size_t			buffer_len;
	t_quote_buffer	qb;

	buffer_len = 0;
	quote = **cmd;
	if (quote != '\'' && quote != '"')
		return (NULL);
	*quote_context = quote;
	(*cmd)++;
	buffer_size = 256;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	qb = (t_quote_buffer){&buffer, &buffer_size, &buffer_len};
	if (quote == '\'')
		return (handle_single_quotes(cmd, &qb));
	else
		return (handle_double_quotes(shell, cmd, &qb, in_del));
}
