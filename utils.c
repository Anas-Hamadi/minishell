#include "parse.h"

void	skip_spaces(char **cmd)
{
	while (**cmd && (**cmd == ' ' || **cmd == '\t'))
		(*cmd)++;
}

int	detect_invalid_metachar(char c)
{
	return (c == ';' || c == '&' ||
			c == '(' || c == ')' || c == '*');
}

char	*handle_quote_block(char **cmd, char *quote_context)
{
	char	quote;
	char	*start;
	char	*buffer;
	int		i = 0;

	quote = **cmd;
	if (quote != '\'' && quote != '"')
		return (NULL);
	*quote_context = quote;
	(*cmd)++; // skip opening quote
	start = *cmd;

	// Find end quote
	while (**cmd && **cmd != quote)
		(*cmd)++;
	if (**cmd != quote)
		return (NULL); // unclosed quote

	// Allocate + copy content between quotes
	buffer = malloc(*cmd - start + 1);
	if (!buffer)
		return (NULL);
	while (start < *cmd)
		buffer[i++] = *start++;
	buffer[i] = '\0';

	(*cmd)++; // skip closing quote
	return (buffer);
}

int	is_metachar(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>' || c == '\0');
}

char	*handle_word(char **cmd)
{
	char	buffer[MAX_WORD_SIZE];
	int		i = 0;
	char	*tmp;
	char	quote;
	char	*quoted;
	char	*expanded;

	while (**cmd && !is_metachar(**cmd))
	{
		if (**cmd == '\'' || **cmd == '"')
		{
			quoted = handle_quote_block(cmd, &quote);
			if (!quoted)
				return (NULL); // unclosed quote
			if (quote == '"') // expand $ inside double quotes
			{
				tmp = quoted;
				quoted = expand_variable(&tmp); // you'll implement this later
				free(tmp);
			}
			for (int j = 0; quoted[j]; j++)
				buffer[i++] = quoted[j];
			free(quoted);
		}
		else if (**cmd == '$')
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

int	get_last_exit_status(void)
{
	return (42);
}
char	*expand_variable(char **cmd)
{
	char	varname[256];
	int		i = 0;
	char	*value;

	if (**cmd != '$')
		return (NULL);
	(*cmd)++; // skip the $

	if (**cmd == '?')
	{
		(*cmd)++;
		value = malloc(12);
		if (!value)
			return (NULL);
		snprintf(value, 12, "%d", get_last_exit_status());
		return (value);
	}

	// valid variable name: starts with letter or _
	if (!isalpha(**cmd) && **cmd != '_')
		return (strdup("")); // invalid var

	// collect variable name
	while (**cmd && (isalnum(**cmd) || **cmd == '_'))
		varname[i++] = *(*cmd)++;
	varname[i] = '\0';

	value = getenv(varname);
	if (!value)
		return (strdup("")); // undefined → expand to empty

	return (strdup(value));
}

