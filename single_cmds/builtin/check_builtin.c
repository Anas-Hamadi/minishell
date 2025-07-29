/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:40:14 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:54:32 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_builtin(t_shell *shell)
{
	if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "cd"))
		ft_cd(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "echo"))
		ft_echo(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "pwd"))
		ft_pwd(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "env"))
		ft_env(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "exit"))
		ft_exit(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "unset"))
		ft_unset(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "export"))
		ft_export(shell);
	else
		return (0);
	return (1);
}
