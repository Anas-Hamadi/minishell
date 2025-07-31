/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:44:55 by molamham          #+#    #+#             */
/*   Updated: 2025/07/31 15:19:56 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	ft_variable_exist(t_list *t_envp, char *var)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = t_envp;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, var, i))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

void	ft_add_ex_var(t_list **t_envp, char *var)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = *t_envp;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, var, i))
		{
			free(tmp->content);
			tmp->content = ft_strdup(var);
			return ;
		}
		tmp = tmp->next;
	}
}

bool	is_valid_identifier(char *str)
{
	int		i;
	char	*equal_pos;

	if (!str || !*str)
		return (false);
	// Find the = sign if it exists
	equal_pos = ft_strchr(str, '=');
	// Check first character (must be letter or underscore)
	if (!isalpha(str[0]) && str[0] != '_')
		return (false);
	// Check remaining characters until = or end of string
	i = 1;
	while (str[i] && (!equal_pos || str + i < equal_pos))
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	print_content(char *str)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(str, 1);
		return ;
	}
	key = ft_substr(str, 0, equal_sign - str);
	value = equal_sign + 1;
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(value, 1);
	ft_putstr_fd("\"\n", 1);
	free(key);
}

void	ft_export(t_shell *shell)
{
	int		y;
	int		i;
	char	**arr;
	char	**input;
	int		exit_code;

	input = shell->cmds->argv;
	i = 0;
	exit_code = 0;
	arr = sorted_env(shell->envp);
	if (!input[1])
	{
		while (arr[i])
			print_content(arr[i++]);
	}
	else
	{
		y = 1;
		while (input[y])
		{
			if (!is_valid_identifier(input[y]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(input[y], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				exit_code = 1;
			}
			else
			{
				if (ft_variable_exist(shell->envp, input[y]))
					ft_add_ex_var(&shell->envp, input[y]);
				else
					ft_lstadd_back(&shell->envp,
									ft_lstnew(ft_strdup(input[y])));
			}
			y++;
		}
	}
	ft_free(arr);
	shell->exit_code = exit_code;
}
