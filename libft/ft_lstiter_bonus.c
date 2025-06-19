/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:16:13 by ahamadi           #+#    #+#             */
/*   Updated: 2024/11/24 14:56:43 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*lst_cpy;

	if (!lst || !f)
		return ;
	lst_cpy = lst;
	while (lst_cpy != 0)
	{
		(*f)(lst_cpy->content);
		lst_cpy = lst_cpy->next;
	}
}
