/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:06:22 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 13:24:25 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_cmds_list(t_cmdnode *cmd_list)
{
	t_cmdnode	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_argv(cmd_list->argv);
		free_redirs(cmd_list->redirs);
		free(cmd_list);
		cmd_list = tmp;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->input)
		free(shell->input);
	if (shell->cmds)
		free_cmds_list(shell->cmds);
	if (shell->envp)
		ft_lstclear(&shell->envp, free);
}

void	ft_lstfree(t_list *lst)
{
	free(lst->content);
	free(lst);
}
