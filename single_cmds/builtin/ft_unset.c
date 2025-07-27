/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:45:24 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 15:45:35 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	var_comp(t_list *var_node, char *var_name)
{
	int		i;
	char	*var;

	i = 0;
	var = (char *) var_node->content;
	while (var[i] == var_name[i] && var_name[i] && var[i] != '=')
		i++;
	if (var_name[i] == '\0')
		return (0);
	return (1);
}

void	delvar(t_list **t_envp, char *var)
{
	t_list	*curr;
	t_list	*tmp;

	curr = *t_envp;
	if (var_comp(*t_envp, var) == 0)
	{
		tmp = *t_envp;
		*t_envp = (*t_envp)->next;
		ft_lstfree(tmp);
		return ;
	}
	while (curr && curr->next)
	{
		if (var_comp(curr->next, var) == 0)
		{
			tmp = curr->next;
			curr->next = curr->next->next;
			ft_lstfree(tmp);
			return ;
		}
		curr = curr->next;
	}
}

char	**set_var_name(char	**s_input)
{
	int		i;
	int		j;
	char	**vars;

	i = 1;
	j = 0;
	while (s_input[i])
		i++;
	vars = malloc(sizeof(char *) * (i + 1));
	i = 1;
	while (s_input[i])
	{
		vars[j] = ft_strdup(s_input[i]);
		i++;
		j++;
	}
	vars[j] = NULL;
	return (vars);
}

int	check_var(t_list	*t_envp, char *var_name)
{
	t_list	*tmp;

	tmp = t_envp;
	while (tmp)
	{
		if (var_comp(tmp, var_name) == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	ft_unset(char **s_input, t_list *t_envp)
{
	int		i;
	char	**var_name;

	i = 0;
	var_name = set_var_name(s_input);
	if (!s_input[1])
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	else
	{
		while (var_name[i])
		{
			if (check_var(t_envp, var_name[i]) == 0)
				delvar(&t_envp, var_name[i]);
			else
				printf("Var not found\n");
			i++;
		}
	}
}
