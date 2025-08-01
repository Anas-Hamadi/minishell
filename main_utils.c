/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:10:33 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 15:11:26 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parse_error(t_shell *shell)
{
	if (g_signal_num == SIGINT)
	{
		shell->exit_code = 130;
	}
	else if (g_signal_num == SIGQUIT)
	{
		shell->exit_code = 131;
	}
	else
	{
		ft_putstr_fd(RED "minishell: parse error\n" RESET, 2);
		shell->exit_code = 2;
	}
}

void	restore_stdio(int saved_in, int saved_out)
{
	dup2(saved_in, 0);
	dup2(saved_out, 1);
	close(saved_in);
	close(saved_out);
}

int	handle_input(t_shell *shell)
{
	if (g_signal_num == 9999)
		shell->exit_code = 130;
	if (!shell->input)
		exit(shell->exit_code);
	if (*shell->input)
	{
		add_history(shell->input);
		g_signal_num = 0;
		start(shell);
		return (1);
	}
	else
	{
		free(shell->input);
		return (0);
	}
}

void	check_signals_after_execution(t_shell *shell)
{
	if (g_signal_num == SIGINT)
	{
		shell->exit_code = 130;
		g_signal_num = 0;
	}
	else if (g_signal_num == SIGQUIT)
	{
		shell->exit_code = 131;
		g_signal_num = 0;
	}
}
