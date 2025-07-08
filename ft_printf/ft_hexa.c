/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:45:13 by molamham          #+#    #+#             */
/*   Updated: 2024/11/26 13:35:25 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_hexa(unsigned long n, char c)
{
	int		count;
	char	*s;

	count = 0;
	s = "0123456789abcdef";
	if (c == 'X')
		s = "0123456789ABCDEF";
	if (n >= 16)
		count += ft_hexa(n / 16, c);
	count += ft_putchar(s[n % 16]);
	return (count);
}
