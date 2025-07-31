/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:26:33 by molamham          #+#    #+#             */
/*   Updated: 2025/07/31 16:35:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_empty_args(char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
		{
			if (i != j)
				argv[j] = argv[i];
			j++;
		}
		i++;
	}
	argv[j] = NULL;
}

void	handle_single_cmd(t_shell *shell)
{
	if (shell->cmds->redirs)
		handle_redirs(shell);
	if (shell->cmds->argv)
		remove_empty_args(shell->cmds->argv);
	if (!shell->cmds->argv || !shell->cmds->argv[0] ||
		shell->cmds->argv[0][0] == '\0')
	{
		shell->exit_code = 0; // Empty command is successful no-op
		return ;
	}
	if (!check_builtin(shell))
		check_exec(shell);
}
