/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:34:29 by molamham          #+#    #+#             */
/*   Updated: 2024/12/09 15:10:39 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_format(char c, va_list list)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar(va_arg(list, int));
	else if (c == 'd' || c == 'i')
		count += ft_putnbr(va_arg(list, int));
	else if (c == 'u')
		count += ft_putnbr(va_arg(list, unsigned int));
	else if (c == 's')
		count += ft_putstr(va_arg(list, char *));
	else if (c == 'x' || c == 'X')
		count += ft_hexa(va_arg(list, unsigned int), c);
	else if (c == 'p')
		count += ft_putptr(va_arg(list, void *));
	else
	{
		count += ft_putchar('%');
		count += ft_putchar(c);
	}
	return (count);
}

int	ft_printf(const char *s, ...)
{
	va_list	list;
	int		count;

	count = 0;
	if (!s)
		return (-1);
	va_start(list, s);
	while (*s)
	{
		if (*s == '%' && *(s + 1) != '\0')
		{
			s++;
			if (*s != '%')
				count += print_format(*s, list);
			else
				count += ft_putchar('%');
		}
		else if (*s != '%')
			count += ft_putchar(*s);
		s++;
	}
	va_end(list);
	return (count);
}
