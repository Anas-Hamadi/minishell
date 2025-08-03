/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:15:16 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/03 12:27:45 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/* Create a new command node */
t_cmdnode	*create_cmdnode(void)
{
	t_cmdnode	*cmd;

	cmd = malloc(sizeof(t_cmdnode));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->argv[0] = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->cmd_has_quotes = false;
	return (cmd);
}

/* Create a new redirection */
struct s_redir	*create_redir(t_redir_type type, char *filename)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

static int	check_syntax_errors(char **cmd_ptr, t_cmdnode *head)
{
	if ((**cmd_ptr == '|' && (*cmd_ptr)[1] == '|') ||
		(**cmd_ptr == '&' && (*cmd_ptr)[1] == '&'))
	{
		write(STDERR_FILENO, "syntax error: operator '", 24);
		write(STDERR_FILENO, *cmd_ptr, 2);
		write(STDERR_FILENO, "' not supported\n", 16);
		free_cmd_list(head);
		return (-1);
	}
	if (detect_invalid_metachar(**cmd_ptr))
	{
		write(STDERR_FILENO, "Error: invalid char '", 21);
		write(STDERR_FILENO, *cmd_ptr, 1);
		write(STDERR_FILENO, "'\n", 2);
		free_cmd_list(head);
		return (-1);
	}
	if (**cmd_ptr == '<' && (*cmd_ptr)[1] == '<' && (*cmd_ptr)[2] == '<')
	{
		write(STDERR_FILENO, "syntax error: '<<<' operator not supported\n",
			44);
		free_cmd_list(head);
		return (-1);
	}
	return (0);
}

static int	process_token(struct s_shell *shell, char **cmd, t_cmdnode **cur,
		t_cmdnode *head)
{
	if (handle_token_types(shell, cmd, cur) < 0)
	{
		free_cmd_list(head);
		return (-1);
	}
	return (0);
}

struct s_cmdnode	*parse_command_line(struct s_shell *shell, char *input)
{
	t_cmdnode	*head;
	t_cmdnode	*cur;
	char		*cmd;

	head = create_cmdnode();
	if (!head)
		return (NULL);
	cur = head;
	cmd = input;
	while (*cmd)
	{
		skip_spaces(&cmd);
		if (!*cmd)
			break ;
		if (check_syntax_errors(&cmd, head) < 0)
			return (NULL);
		if (process_token(shell, &cmd, &cur, head) < 0)
			return (NULL);
	}
	return (head);
}
