/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:14:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:40:55 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

static char	*handle_special_var(struct s_shell *shell, char **cmd)
{
	char	*value;

	if (**cmd == '?')
	{
		(*cmd)++;
		value = ft_itoa_simple(get_last_exit_status(shell));
		if (!value)
			return (NULL);
		return (value);
	}
	if (!ft_isalpha(**cmd) && **cmd != '_')
		return (ft_strdup("$"));
	return (NULL);
}

static char	*extract_varname(char **cmd)
{
	char	*varname;
	size_t	varname_size;
	size_t	varname_len;

	varname_len = 0;
	varname_size = 64;
	varname = malloc(varname_size);
	if (!varname)
		return (NULL);
	varname[0] = '\0';
	while (**cmd && (ft_isalnum(**cmd) || **cmd == '_')
		&& !is_metachar(**cmd))
	{
		if (!safe_charcat_realloc(&varname, &varname_size, &varname_len, **cmd))
			return (NULL);
		(*cmd)++;
	}
	return (varname);
}

char	*expand_variable(struct s_shell *shell, char **cmd)
{
	char	*varname;
	char	*value;

	if (**cmd != '$')
		return (NULL);
	(*cmd)++;
	value = handle_special_var(shell, cmd);
	if (value)
		return (value);
	varname = extract_varname(cmd);
	if (!varname)
		return (NULL);
	value = get_env_value(shell, varname);
	free(varname);
	if (!value)
		return (ft_strdup(""));
	return (value);
}
