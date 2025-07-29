/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:44:55 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 17:39:38 by molamham         ###   ########.fr       */
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

	input = shell->cmds->argv;
	i = 0;
	arr = sorted_env(shell->envp);
	if (!input[1])
		while (arr[i])
			print_content(arr[i++]);
	else
	{
		y = 1;
		while (input[y])
		{
			if (ft_variable_exist(shell->envp, input[y]))
				ft_add_ex_var(&shell->envp, input[y]);
			else
				ft_lstadd_back(&shell->envp, ft_lstnew(ft_strdup(input[y])));
			y++;
		}
	}
	ft_free(arr);
	shell->exit_code = 0;
}
