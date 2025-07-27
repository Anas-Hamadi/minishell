/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:19:40 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 13:19:41 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_lstadd_back(t_list **lst, t_list *t_new)
{
	t_list	*last;

	if (!lst || !t_new)
		return ;
	if (!*lst)
		*lst = t_new;
	else
	{
		last = ft_lstlast(*lst);
		last->next = t_new;
	}
}
