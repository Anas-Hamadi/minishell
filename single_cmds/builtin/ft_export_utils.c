/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 21:32:11 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 22:43:45 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	ft_variable_exist(t_list *t_envp, char *var)
{
	t_list	*tmp;
	char	*env_var;
	char	*equal_pos;
	int		var_len;

	equal_pos = ft_strchr(var, '=');
	if (equal_pos)
		var_len = equal_pos - var;
	else
		var_len = ft_strlen(var);
	tmp = t_envp;
	while (tmp)
	{
		env_var = (char *)tmp->content;
		if (ft_strncmp(env_var, var, var_len) == 0
			&& (env_var[var_len] == '=' || env_var[var_len] == '\0'))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

bool	is_valid_identifier(char *str)
{
	int		i;
	char	*equal_pos;

	if (!str || !*str)
		return (false);
	equal_pos = ft_strchr(str, '=');
	if (!isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && (!equal_pos || str + i < equal_pos))
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
