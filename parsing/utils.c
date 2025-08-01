/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:05:31 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 15:31:54 by ahamadi          ###   ########.fr       */
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
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>' ||
			c == '\0');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
			c == '\r');
}

int	detect_invalid_metachar(char c)
{
	return ((c == ';' || c == '&' || c == '(' || c == ')' || c == '*'));
}

char	*handle_quote_block(struct s_shell *shell, char **cmd,
		char *quote_context, bool in_del)
{
	char	quote;
	char	*buffer;
	size_t	buffer_size;
	size_t	buffer_len;
	char	*expanded;

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
	if (quote == '\'')
	{
		while (**cmd && **cmd != '\'')
		{
			if (!safe_charcat_realloc(&buffer, &buffer_size, &buffer_len,
					**cmd))
				return (NULL);
			(*cmd)++;
		}
		if (**cmd != '\'')
		{
			free(buffer);
			return (NULL);
		}
		(*cmd)++;
		return (buffer);
	}
	else
	{
		while (**cmd && **cmd != '"')
		{
			if (**cmd == '$' && !in_del)
			{
				expanded = expand_variable(shell, cmd);
				if (!expanded)
				{
					free(buffer);
					return (NULL);
				}
				if (!safe_strcat_realloc(&buffer, &buffer_size, &buffer_len,
						expanded))
				{
					free(expanded);
					return (NULL);
				}
				free(expanded);
			}
			else
			{
				if (!safe_charcat_realloc(&buffer, &buffer_size, &buffer_len,
						**cmd))
					return (NULL);
				(*cmd)++;
			}
		}
		if (**cmd != '"')
		{
			free(buffer);
			return (NULL);
		}
		(*cmd)++;
		return (buffer);
	}
}

char	*handle_hd_line(t_shell *shell, char **cmd)
{
	char	*buffer;
	size_t	buffer_size;
	size_t	buffer_len;
	char	*expanded;
	char	*varname_start;
	char	saved;
	char	*env_value;

	buffer_len = 0;
	buffer_size = 256;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	while (**cmd)
	{
		if (**cmd == '$')
		{
			(*cmd)++;
			if (**cmd == '?')
			{
				(*cmd)++;
				expanded = ft_itoa_simple(shell->exit_code);
			}
			else
			{
				varname_start = *cmd;
				while (**cmd && (isalnum(**cmd) || **cmd == '_'))
					(*cmd)++;
				if (*cmd > varname_start)
				{
					saved = **cmd;
					**cmd = '\0';
					env_value = get_env_value(shell, varname_start);
					**cmd = saved;
					expanded = env_value ? strdup(env_value) : strdup("");
				}
				else
				{
					expanded = strdup("");
				}
			}
			if (!expanded)
			{
				free(buffer);
				return (NULL);
			}
			if (!safe_strcat_realloc(&buffer, &buffer_size, &buffer_len,
					expanded))
			{
				free(expanded);
				return (NULL);
			}
			free(expanded);
		}
		else
		{
			if (!safe_charcat_realloc(&buffer, &buffer_size, &buffer_len,
					**cmd))
				return (NULL);
			(*cmd)++;
		}
	}
	return (buffer);
}

char	*handle_word(struct s_shell *shell, char **cmd, bool in_del,
		bool *expand_in_hd)
{
	char	*buffer;
	size_t	buffer_size;
	size_t	buffer_len;
	char	quote;
	char	*quoted;
	char	*expanded;
	bool	found_quotes;

	buffer_len = 0;
	found_quotes = false;
	buffer_size = 256;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	while (**cmd && !is_metachar(**cmd))
	{
		if (**cmd == '\'' || **cmd == '"')
		{
			quoted = handle_quote_block(shell, cmd, &quote, in_del);
			if (!quoted)
			{
				free(buffer);
				return (NULL);
			}
			if (!safe_strcat_realloc(&buffer, &buffer_size, &buffer_len,
					quoted))
			{
				free(quoted);
				return (NULL);
			}
			found_quotes = true;
			free(quoted);
		}
		else if (**cmd == '$' && !in_del)
		{
			expanded = expand_variable(shell, cmd);
			if (!expanded)
			{
				free(buffer);
				return (NULL);
			}
			if (!safe_strcat_realloc(&buffer, &buffer_size, &buffer_len,
					expanded))
			{
				free(expanded);
				return (NULL);
			}
			free(expanded);
		}
		else
		{
			if (!safe_charcat_realloc(&buffer, &buffer_size, &buffer_len,
					**cmd))
				return (NULL);
			(*cmd)++;
		}
	}
	if (expand_in_hd && found_quotes)
		*expand_in_hd = false;
	return (buffer);
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

char	*get_env_value(struct s_shell *shell, const char *key)
{
	t_list	*tmp;
	char	*content;
	size_t	key_len;

	if (!key || !shell || !shell->envp)
		return (NULL);
	key_len = strlen(key);
	tmp = shell->envp;
	while (tmp)
	{
		content = (char *)tmp->content;
		if (content && strncmp(content, key, key_len) == 0 &&
			content[key_len] == '=')
		{
			return (strdup(content + key_len + 1));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	get_last_exit_status(struct s_shell *shell)
{
	return (shell->exit_code);
}
char	*expand_variable(struct s_shell *shell, char **cmd)
{
	char	*varname;
	size_t	varname_size;
	size_t	varname_len;
	char	*value;

	varname_len = 0;
	if (**cmd != '$')
		return (NULL);
	(*cmd)++;
	if (**cmd == '?')
	{
		(*cmd)++;
		value = ft_itoa_simple(get_last_exit_status(shell));
		if (!value)
			return (NULL);
		return (value);
	}
	if (!isalpha(**cmd) && **cmd != '_')
	{
		return (strdup("$"));
	}
	varname_size = 64;
	varname = malloc(varname_size);
	if (!varname)
		return (NULL);
	varname[0] = '\0';
	while (**cmd && (isalnum(**cmd) || **cmd == '_') && !is_metachar(**cmd))
	{
		if (!safe_charcat_realloc(&varname, &varname_size, &varname_len, **cmd))
			return (NULL);
		(*cmd)++;
	}
	value = get_env_value(shell, varname);
	free(varname);
	if (!value)
		return (strdup(""));
	return (value);
}
