/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:29:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/02 20:07:07 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

int	check_delimiter_match(const char *line, const char *delimiter)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter))
		return (1);
	return (0);
}

int	process_heredoc_line(int fd, const char *delimiter, int *expand,
		struct s_shell *shell)
{
	char	*line;
	char	*processed_line;

	line = readline("> ");
	if (!line)
	{
		write(STDOUT_FILENO, "here-document delimited by end-of-file\n", 39);
		return (0);
	}
	if (check_delimiter_match(line, delimiter))
	{
		free(line);
		return (0);
	}
	processed_line = process_line_expansion(shell, line, expand);
	if (!processed_line)
		return (-1);
	if (write_line_to_file(fd, processed_line) < 0)
	{
		free(processed_line);
		return (-1);
	}
	free(processed_line);
	return (1);
}

int	read_heredoc_lines(int fd, const char *delimiter, int *expand,
		struct s_shell *shell)
{
	int	result;

	while (1)
	{
		result = process_heredoc_line(fd, delimiter, expand, shell);
		if (result <= 0)
			return (result);
	}
}
