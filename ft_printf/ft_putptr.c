/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:22:00 by molamham          #+#    #+#             */
/*   Updated: 2024/11/26 13:40:21 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putptr(void *p)
{
	int				count;
	unsigned long	n;

	count = 0;
	if (!p)
		return (ft_putstr("(nil)"));
	count += ft_putstr("0x");
	n = (unsigned long)p;
	count += ft_hexa(n, 'x');
	return (count);
}
