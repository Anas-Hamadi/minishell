/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:46:55 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 15:13:49 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_redir(char *filename, int flags, int std_fd)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror(filename);
		return (-1);
	}
	dup2(fd, std_fd);
	close(fd);
	return (0);
}

int	handle_redirs(t_shell *shell)
{
	t_redir	*redirs;

	redirs = shell->cmds->redirs;
	while (redirs)
	{
		if (redirs->type == R_IN)
		{
			if (handle_single_redir(redirs->filename, O_RDONLY, 0) < 0)
				return (-1);
		}
		if (redirs->type == R_OUT)
		{
			if (handle_single_redir(redirs->filename,
					O_WRONLY | O_CREAT | O_TRUNC, 1) < 0)
				return (-1);
		}
		else if (redirs->type == R_APPEND)
		{
			if (handle_single_redir(redirs->filename,
					O_WRONLY | O_CREAT | O_APPEND, 1) < 0)
				return (-1);
		}
		redirs = redirs->next;
	}
	return (0);
}
