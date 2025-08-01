/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:21:41 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:31:51 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft/libft.h"
#include "parse.h"

static int	validate_and_get_delimiter(struct s_shell *shell, char **cmd_ptr,
		char **delim, bool *expand_hd)
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

static int	process_heredoc_content(struct s_shell *shell, char *delim,
		bool expand_hd, char **tmp)
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

int	handle_heredoc_token(struct s_shell *shell, char **cmd_ptr, t_cmdnode *cur)
{
	char	*delim;
	char	*tmp;
	bool	expand_hd;
	t_redir	*r;

	*cmd_ptr += 2;
	skip_spaces(cmd_ptr);
	if (!**cmd_ptr)
	{
		write(STDERR_FILENO, "syntax error: expected heredoc delimiter\n", 42);
		return (-1);
	}
	if (validate_and_get_delimiter(shell, cmd_ptr, &delim, &expand_hd) < 0)
		return (-1);
	if (process_heredoc_content(shell, delim, expand_hd, &tmp) < 0)
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

static int	process_redirection_filename(struct s_shell *shell, char **cmd_ptr,
		t_cmdnode *cur, t_redir_type type)
{
	char	*filename;
	t_redir	*r;

	if (!**cmd_ptr)
	{
		write(STDERR_FILENO,
			"syntax error: expected filename after redirection\n",
			51);
		return (-1);
	}
	filename = handle_word(shell, cmd_ptr, 0, 0);
	if (!filename)
	{
		write(STDERR_FILENO, "syntax error: bad filename\n", 28);
		return (-1);
	}
	r = create_redir(type, filename);
	if (r)
		add_redir_to_cmd(cur, r);
	free(filename);
	return (0);
}

int	handle_redirection_token(struct s_shell *shell, char **cmd_ptr,
		t_cmdnode *cur)
{
	t_redir_type	type;
	char			op;

	if (**cmd_ptr == '<')
		type = R_IN;
	else
		type = R_OUT;
	op = *(*cmd_ptr)++;
	if (**cmd_ptr == op)
	{
		type = R_APPEND;
		(*cmd_ptr)++;
	}
	skip_spaces(cmd_ptr);
	return (process_redirection_filename(shell, cmd_ptr, cur, type));
}
