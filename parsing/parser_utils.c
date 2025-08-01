/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:22:26 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 17:20:57 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	copy_argv_array(char **new_argv, char **old_argv, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
}

void	add_arg_to_cmd(t_cmdnode *cmd, char *arg)
{
	char	**new_argv;
	int		count;

	count = 0;
	while (cmd->argv && cmd->argv[count])
		count++;
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return ;
	copy_argv_array(new_argv, cmd->argv, count);
	new_argv[count] = ft_strdup(arg);
	if (!new_argv[count])
	{
		free(new_argv);
		return ;
	}
	new_argv[count + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

void	add_redir_to_cmd(t_cmdnode *cmd, t_redir *redir)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = redir;
}
