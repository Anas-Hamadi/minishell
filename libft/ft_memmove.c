/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:11:26 by ahamadi           #+#    #+#             */
/*   Updated: 2024/11/24 15:00:43 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, void *src, size_t len)
{
	char	*dest1;
	char	*src1;
	size_t	i;

	if (!dest || !src)
		return (NULL);
	dest1 = (char *)dest;
	src1 = (char *)src;
	if (dest <= src)
	{
		while (len--)
			*dest1++ = *src1++;
	}
	else if (dest > src)
	{
		i = len;
		while (i--)
		{
			dest1[i] = src1[i];
		}
	}
	return (dest);
}
