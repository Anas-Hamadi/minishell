/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:21:33 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/02 20:00:10 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

int	validate_heredoc_delimiter(const char *delim)
{
	if (delim[0] == '<' || delim[0] == '>' || delim[0] == '|' || delim[0] == '&'
		|| delim[0] == ';' || delim[0] == '(' || delim[0] == ')'
		|| delim[0] == '*')
	{
		write(STDERR_FILENO, "syntax error: invalid heredoc delimiter\n", 40);
		return (0);
	}
	return (1);
}

static int	get_heredoc_delimiter(struct s_shell *shell, char **cmd_ptr,
		char **delim, int *expand_hd)
{
	*expand_hd = true;
	*delim = handle_word(shell, cmd_ptr, true, expand_hd);
	if (!*delim || (*delim)[0] == '\0')
	{
		write(STDERR_FILENO, "syntax error: bad heredoc delimiter\n", 37);
		if (*delim)
			free(*delim);
		return (-1);
	}
	if (!validate_heredoc_delimiter(*delim))
	{
		free(*delim);
		return (-1);
	}
	return (0);
}

static int	execute_heredoc_processing(struct s_shell *shell, char *delim,
		int *expand_hd, char **tmp)
{
	if (handle_heredoc(shell, delim, expand_hd, tmp) < 0)
	{
		if (g_signal_num == SIGINT)
			return (-1);
		perror("heredoc");
		return (-1);
	}
	return (0);
}

int	handle_heredoc_parsing(struct s_shell *shell, char **cmd_ptr,
		t_cmdnode *cur)
{
	char	*delim;
	char	*tmp;
	int		expand_hd;
	t_redir	*r;

	*cmd_ptr += 2;
	skip_spaces(cmd_ptr);
	if (!**cmd_ptr)
	{
		write(STDERR_FILENO, "syntax error: expected heredoc delimiter\n", 42);
		return (-1);
	}
	if (get_heredoc_delimiter(shell, cmd_ptr, &delim, &expand_hd) < 0)
		return (-1);
	if (execute_heredoc_processing(shell, delim, &expand_hd, &tmp) < 0)
	{
		free(delim);
		return (-1);
	}
	r = create_redir(R_IN, tmp);
	if (r)
		add_redir_to_cmd(cur, r);
	free(delim);
	free(tmp);
	return (0);
}
