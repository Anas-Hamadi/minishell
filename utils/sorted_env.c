/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorted_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:20:59 by molamham          #+#    #+#             */
/*   Updated: 2025/07/27 20:56:59 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	swap_value(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

int	var_name_comp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] != '=' && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s1[i] == '=' && s2[i] != '=')
		return (-1);
	if (s1[i] != '=' && s2[i] == '=')
		return (1);
	return (0);
}

char	**sorted_env(t_list *env)
{
	int		i;
	char	**arr;
	t_list	*tmp;

	tmp = env;
	arr = malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	i = 0;
	while (arr[i] && arr[i + 1])
	{
		if (var_name_comp(arr[i], arr[i + 1]) > 0)
		{
			swap_value(&arr[i], &arr[i + 1]);
			i = -1;
		}
		i++;
	}
	return (arr);
}
