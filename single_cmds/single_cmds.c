/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:26:33 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 13:26:54 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_single_cmd(t_shell *shell)
{
	if (shell->cmds->redirs)
		handle_redirs(shell);
	if (!shell->cmds->argv || !shell->cmds->argv[0])
		return ;
	if (!check_builtin(shell))
		check_exec(shell);
}
