/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:23:15 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 22:44:25 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_exec_error(char **env_array, char *s_input, int exit_code)
{
	ft_putstr_fd(RED "minishell: " RESET, 2);
	ft_putstr_fd(s_input, 2);
	if (exit_code == 127)
		ft_putendl_fd(": No such file or directory" RESET, 2);
	else if (exit_code == 126)
		ft_putendl_fd(": Permission denied" RESET, 2);
	ft_free(env_array);
	exit(exit_code);
}

void	handle_cmd_not_found(t_shell *shell)
{
	ft_putstr_fd("minishell: ", 2);
	if (shell->cmds->argv[0][0] == '\0')
	{
		ft_putendl_fd(": command not found", 2);
	}
	else
	{
		ft_putstr_fd(shell->cmds->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
	}
	shell->exit_code = 127;
}
