/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:26:33 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 21:16:21 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_nonempty_after_empty(char **argv)
{
	int	i;
	int	k;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
		{
			k = i + 1;
			while (argv[k])
			{
				if (argv[k][0] != '\0')
					return (1);
				k++;
			}
		}
		i++;
	}
	return (0);
}

static void	compact_args(char **argv)
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

void	remove_empty_args(char **argv)
{
	if (!has_nonempty_after_empty(argv))
		return ;
	compact_args(argv);
}

void	handle_single_cmd(t_shell *shell)
{
	if (shell->cmds->redirs)
	{
		if (handle_redirs(shell) < 0)
		{
			shell->exit_code = 1;
			return ;
		}
	}
	if (shell->cmds->argv && !shell->cmds->cmd_has_quotes)
		remove_empty_args(shell->cmds->argv);
	if (!shell->cmds->argv || !shell->cmds->argv[0])
	{
		shell->exit_code = 0;
		return ;
	}
	if (!check_builtin(shell))
		check_exec(shell);
}
