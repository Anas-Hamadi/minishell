/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:43:10 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 15:44:16 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	check_flag(char **args, int *i)
{
	int	j;

	if (!args[*i])
		return (true);
	j = 0;
	if (args[1][j++] != '-')
		return (true);
	while (args[1][j] == 'n')
		j++;
	if (args[1][j])
		return (true);
	(*i)++;
	return (false);
}

static bool	handle_new_line(char **args, int *i)
{
	bool	print_new_line;
	int		j;

	print_new_line = check_flag(args, i);
	if (print_new_line == false)
	{
		while (args[*i])
		{
			j = 0;
			if (args[*i][0] != '-')
				break ;
			j++;
			while (args[*i][j] == 'n')
				j++;
			if (args[*i][j])
				break ;
			(*i)++;
		}
	}
	return (print_new_line);
}

void	ft_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = handle_new_line(args, &i);
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1] != NULL)
			ft_putchar(' ', 1);
		i++;
	}
	if (newline == true)
		ft_putchar('\n', 1);
}
