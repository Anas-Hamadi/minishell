/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 21:11:27 by ahamadi           #+#    #+#             */
/*   Updated: 2024/11/24 16:13:58 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*c;
	int		i;

	i = 0;
	if (!lst)
		return (0);
	c = lst;
	while (c)
	{
		c = c->next;
		i++;
	}
	return (i);
}
