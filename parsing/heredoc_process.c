/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:18:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/02 20:01:55 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

char	*process_line_expansion(struct s_shell *shell, char *line, int *expand)
{
	char	*line_copy;
	char	*expanded;

	if (!*(expand))
		return (line);
	line_copy = line;
	expanded = handle_hd_line(shell, &line_copy);
	if (expanded)
	{
		free(line);
		return (expanded);
	}
	else
	{
		free(line);
		return (NULL);
	}
}

int	write_line_to_file(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) < 0)
		return (-1);
	if (write(fd, "\n", 1) < 0)
		return (-1);
	return (0);
}

int	heredoc_child_process(const char *delimiter, int *expand,
		const char *filename, struct s_shell *shell)
{
	int	fd;

	signal(SIGINT, SIG_DFL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (-1);
	if (read_heredoc_lines(fd, delimiter, expand, shell) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	exit(0);
}
