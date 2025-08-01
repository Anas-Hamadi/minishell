/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:44:55 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 22:46:26 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static void	print_all_exports(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		print_content(arr[i++]);
}

static int	handle_export_arg(t_shell *shell, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	else
	{
		if (ft_variable_exist(shell->envp, arg))
			ft_add_ex_var(&shell->envp, arg);
		else
			ft_lstadd_back(&shell->envp, ft_lstnew(ft_strdup(arg)));
	}
	return (0);
}

void	ft_export(t_shell *shell)
{
	int		y;
	char	**arr;
	char	**input;
	int		exit_code;

	input = shell->cmds->argv;
	exit_code = 0;
	arr = sorted_env(shell->envp);
	if (!input[1])
	{
		print_all_exports(arr);
	}
	else
	{
		y = 1;
		while (input[y])
		{
			exit_code += handle_export_arg(shell, input[y]);
			y++;
		}
	}
	ft_free(arr);
	shell->exit_code = exit_code;
}
