/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:05:31 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:27:28 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

void	skip_spaces(char **cmd)
{
	while (**cmd && (**cmd == ' ' || **cmd == '\t'))
		(*cmd)++;
}

int	is_metachar(char c)
{
	if (c == ' ' || c == '\t' || c == '|' || c == '<')
		return (1);
	if (c == '>' || c == '\0')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v')
		return (1);
	if (c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	detect_invalid_metachar(char c)
{
	return ((c == ';' || c == '&' || c == '(' || c == ')' || c == '*'));
}

bool	word_has_quotes(char *input)
{
	char	*cmd;

	cmd = input;
	while (*cmd && !is_metachar(*cmd))
	{
		if (*cmd == '\'' || *cmd == '"')
			return (true);
		cmd++;
	}
	return (false);
}
