/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:21:54 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:41:21 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	process_redir_filename(struct s_shell *shell, char **cmd,
		t_cmdnode *cur, t_redir_type type)
{
	char	*filename;
	t_redir	*redir;

	if (!**cmd)
	{
		write(STDERR_FILENO,
			"syntax error: expected filename after redirection\n",
			51);
		return (-1);
	}
	filename = handle_word(shell, cmd, 0, 0);
	if (!filename)
	{
		write(STDERR_FILENO, "syntax error: bad filename\n", 28);
		return (-1);
	}
	redir = create_redir(type, filename);
	if (redir)
		add_redir_to_cmd(cur, redir);
	free(filename);
	return (0);
}

int	handle_redirection_parsing(struct s_shell *shell, char **cmd,
		t_cmdnode *cur)
{
	t_redir_type	type;
	char			op;

	if (**cmd == '<')
		type = R_IN;
	else
		type = R_OUT;
	op = *(*cmd)++;
	if (**cmd == op)
	{
		type = R_APPEND;
		(*cmd)++;
	}
	skip_spaces(cmd);
	return (process_redir_filename(shell, cmd, cur, type));
}
