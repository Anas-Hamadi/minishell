/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:46:55 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 12:48:24 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_single_redir(char *filename, int flags, int std_fd)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
		perror("open");
	else
	{
		dup2(fd, std_fd);
		close(fd);
	}
}

void	handle_redirs(t_shell *shell)
{
	t_redir	*redirs;

	redirs = shell->cmds->redirs;
	while (redirs)
	{
		if (redirs->type == R_IN)
			handle_single_redir(redirs->filename, O_RDONLY, 0);
		else if (redirs->type == R_OUT)
			handle_single_redir(redirs->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (redirs->type == R_APPEND)
			handle_single_redir(redirs->filename,
				O_WRONLY | O_CREAT | O_APPEND, 1);
		redirs = redirs->next;
	}
}
