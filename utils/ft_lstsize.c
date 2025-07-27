/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:20:16 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 13:23:22 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_lstsize(t_list *lst)
{
	int	size;

	size = 0;
	if (!lst)
		return (0);
	while (lst != NULL)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}
