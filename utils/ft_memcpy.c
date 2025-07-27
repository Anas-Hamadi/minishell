/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:20:22 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 13:20:23 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_memcpy(void	*dest, const void *src, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;
	size_t			i;

	i = 0;
	s1 = (unsigned char *)dest;
	s2 = (unsigned char *)src;
	if (dest != src)
	{
		while (i < n)
		{
			s1[i] = s2[i];
			i++;
		}
	}
	return (dest);
}
