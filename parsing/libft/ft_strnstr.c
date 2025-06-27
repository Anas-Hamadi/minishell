/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:20:50 by ahamadi           #+#    #+#             */
/*   Updated: 2024/11/24 15:10:00 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *need, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (need[j] == 0)
		return ((char *)hay);
	if (hay == NULL && len == 0)
		return (NULL);
	while (hay[i] && i < len)
	{
		j = 0;
		while (hay[i + j] == need[j] && hay[i + j] && (i + j) < len)
		{
			j++;
		}
		if (need[j] == 0)
			return ((char *)hay + i);
		i++;
	}
	return (NULL);
}
