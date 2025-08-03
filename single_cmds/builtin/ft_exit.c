/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:44:30 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 22:37:38 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isvalid_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

static void	ft_exit_error(t_shell *shell, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	if (shell->stdio_saved)
	{
		restore_stdio(shell->saved_in, shell->saved_out);
		shell->stdio_saved = 0;
	}
	free_shell(shell);
	exit(2);
}

void	ft_exit(t_shell *shell)
{
	char		**argv;
	long long	exit_code;

	argv = shell->cmds->argv;
	exit_code = shell->exit_code;
	ft_putendl_fd("exit", 1);
	if (argv[1] && !ft_isvalid_arg(argv[1]))
		ft_exit_error(shell, argv[1]);
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		shell->exit_code = 1;
		return ;
	}
	if (argv[1])
		exit_code = ft_atoi(argv[1]);
	if (shell->stdio_saved)
	{
		restore_stdio(shell->saved_in, shell->saved_out);
		shell->stdio_saved = 0;
	}
	free_shell(shell);
	exit(exit_code);
}
