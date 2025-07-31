/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:45:46 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:56:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_env(t_shell *shell)
{
	t_list	*tmp;

	tmp = shell->envp;
	while (tmp)
	{
		if (ft_strchr((char *)tmp->content, '='))
			ft_putendl_fd((char *)tmp->content, 1);
		tmp = tmp->next;
	}
	shell->exit_code = 0;
}
