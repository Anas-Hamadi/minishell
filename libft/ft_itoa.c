/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 00:53:27 by ahamadi           #+#    #+#             */
/*   Updated: 2024/11/24 15:24:09 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_len(long n)
{
	int	i;

	i = 0;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*n_positive(long n)
{
	int		len;
	char	*str;

	len = count_len(n);
	str = (char *) malloc (sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	while (len >= 0)
	{
		str[len] = (n % 10) + 48;
		n /= 10;
		len--;
	}
	return (str);
}

static char	*n_negative(long n)
{
	int		len;
	char	*str;

	n *= -1;
	len = count_len(n);
	str = (char *) malloc (sizeof(char) * (len + 2));
	if (!str)
		return (NULL);
	str[len + 1] = '\0';
	while (len > 0)
	{
		str[len] = (n % 10) + 48;
		n /= 10;
		len--;
	}
	str[len] = '-';
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	i;

	i = n;
	if (i == 0)
	{
		str = malloc (2 * sizeof(char));
		if (!str)
			return (NULL);
		str[0] = '0';
		str[1] = '\0';
	}
	else if (i == -2147483648)
		str = n_negative(i);
	else if (n > 0)
		str = n_positive(i);
	else
		str = n_negative(i);
	return (str);
}
