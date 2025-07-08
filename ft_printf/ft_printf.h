/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:10:08 by molamham          #+#    #+#             */
/*   Updated: 2024/12/09 15:14:51 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>

int	ft_putchar(int c);
int	ft_putstr(char *s);
int	ft_putnbr(long n);
int	ft_printf(const char *s, ...);
int	ft_hexa(unsigned long int n, char c);
int	ft_putptr(void *p);

#endif