/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:45:46 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 15:45:47 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_env(t_list *t_envp)
{
	t_list	*tmp;

	tmp = t_envp;
	while (tmp)
	{
		if (ft_strchr((char *)tmp->content, '='))
			printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}
