/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:22:16 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 17:20:55 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	free_redir_list(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->filename);
		free(redirs);
		redirs = next;
	}
}

void	free_cmdnode(t_cmdnode *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	if (cmd->argv)
	{
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
	free_redir_list(cmd->redirs);
	free(cmd);
}

void	free_cmd_list(t_cmdnode *cmd_list)
{
	t_cmdnode	*next;

	while (cmd_list)
	{
		next = cmd_list->next;
		free_cmdnode(cmd_list);
		cmd_list = next;
	}
}
