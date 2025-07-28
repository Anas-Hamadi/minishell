#include "parse.h"

void	skip_spaces(char **cmd)
{
	while (**cmd && (**cmd == ' ' || **cmd == '\t'))
		(*cmd)++;
}

int is_metachar(char c)
{
	return (c== ' ' || c == '\t' || c == '|' || c == '<' || c == '>' || c == '\0');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}

int	detect_invalid_metachar(char c)
{
	return (c == ';' || c == '&' ||
			c == '(' || c == ')' || c == '*');
}

char	*handle_quote_block(char **cmd, char *quote_context, bool in_del)
{
	char	quote;
	char	buffer[MAX_WORD_SIZE]; // todo: char	*buffer; to be malloc'ed
	int		i = 0;

	quote = **cmd;
	if (quote != '\'' && quote != '"')
		return (NULL);
	*quote_context = quote;
	(*cmd)++; // skip opening quote

	if (quote == '\'')
	{
		// Single quotes: copy literally until next single quote
		while (**cmd && **cmd != '\'')
			buffer[i++] = *(*cmd)++;
		if (**cmd != '\'')
			return (NULL); // unclosed quote
		(*cmd)++; // skip closing quote
		buffer[i] = '\0';
		return (strdup(buffer));
	}
	else // double quotes: expand variables
	{
		while (**cmd && **cmd != '"')
		{
			if (**cmd == '$' && !in_del)
			{
				char *expanded = expand_variable(cmd);
				if (!expanded)
					return (NULL);
				for (int j = 0; expanded[j]; j++)
					buffer[i++] = expanded[j];
				free(expanded);
			}
			else
			{
				buffer[i++] = *(*cmd)++;
			}
		}
		if (**cmd != '"')
			return (NULL); // unclosed quote
		(*cmd)++; // skip closing quote
		buffer[i] = '\0';
		return (strdup(buffer));
	}
}

char	*handle_hd_line(char **cmd)
{
	char	buffer[(MAX_WORD_SIZE * 10)]; // todo: malloc
	int		i = 0;
	char	*expanded;

	while (**cmd)
	{
		if (**cmd == '$')
		{
			expanded = expand_variable(cmd);
			if (!expanded)
				return (NULL);
			for (int j = 0; expanded[j]; j++)
				buffer[i++] = expanded[j];
			free(expanded);
		}
		else
		{
			buffer[i++] = **cmd;
			(*cmd)++;
		}
	}
	buffer[i] = '\0'; 
	return (strdup(buffer));
}


char	*handle_word(char **cmd, bool in_del, bool *expand_in_hd)
{
	char	buffer[MAX_WORD_SIZE]; // todo: char	*buffer; to be malloc'ed
	int		i = 0;
	char	quote;
	char	*quoted;
	char	*expanded;
	bool	found_quotes = false;

	while (**cmd && !is_metachar(**cmd))
	{
		if (**cmd == '\'' || **cmd == '"')
		{
			quoted = handle_quote_block(cmd, &quote, in_del);
			if (!quoted)
				return (NULL); // unclosed quote
			for (int j = 0; quoted[j]; j++)
				buffer[i++] = quoted[j];
			found_quotes = true;
			free(quoted);
		}
		else if (**cmd == '$' && !in_del)
		{
			expanded = expand_variable(cmd);
			if (!expanded)
				return (NULL);
			for (int j = 0; expanded[j]; j++)
				buffer[i++] = expanded[j];
			free(expanded);
		}
		else
		{
			buffer[i++] = **cmd;
			(*cmd)++;
		}
	}
	buffer[i] = '\0';

	// If we found quotes, signal that expansion should be disabled
	if (expand_in_hd && found_quotes)
		*expand_in_hd = false;

	return (strdup(buffer));
}

int	get_last_exit_status(void)
{
	return (0); // todo: work with the struct int exit_code var
}
char	*expand_variable(char **cmd)
{
	char	varname[256]; //? confirm if rly a var can't exceed 256 chars
	int		i = 0;
	char	*value;
	// char	*mark;

	// mark = *cmd;
	if (**cmd != '$')
		return (NULL);
	(*cmd)++; // skip the $

	if (**cmd == '?')
	{
		(*cmd)++;
		value = malloc(12);
		if (!value)
			return (NULL);

		// WARNING: snprintf not allowed
		snprintf(value, 12, "%d", get_last_exit_status()); // todo: no snprintf

		// free(mark);
		return (value);
	}

	// valid variable name: starts with letter or _
	if (!isalpha(**cmd) && **cmd != '_')
		return (strdup("")); // invalid var

	// collect variable name
	while (**cmd && (isalnum(**cmd) || **cmd == '_') && !is_metachar(**cmd))
		varname[i++] = *(*cmd)++;
	varname[i] = '\0';

	value = getenv(varname);
	if (!value)
		return (strdup("")); // undefined → expand to empty

	return (strdup(value));
}

