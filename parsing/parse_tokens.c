/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:22:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/03 12:22:51 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft/libft.h"
#include "parse.h"

int	handle_pipe_token(char **cmd_ptr, t_cmdnode **cur)
{
	(*cmd_ptr)++;
	(*cur)->next = create_cmdnode();
	if (!(*cur)->next)
		return (-1);
	*cur = (*cur)->next;
	return (0);
}

int	handle_word_token(struct s_shell *shell, char **cmd_ptr, t_cmdnode *cur)
{
	char	*word;
	char	*end;
	bool	has_quotes;
	int		expand;

	expand = 1;
	has_quotes = word_has_quotes(*cmd_ptr);
	word = handle_word(shell, cmd_ptr, 0, &expand);
	if (!word)
	{
		write(STDERR_FILENO, "Parse error\n", 12);
		return (-1);
	}
	if (!has_quotes)
	{
		end = word + ft_strlen(word) - 1;
		while (end >= word && (*end == ' ' || *end == '\t'))
			*end-- = '\0';
	}
	if (has_quotes && (!cur->argv || !cur->argv[0]))
		cur->cmd_has_quotes = true;
	add_arg_to_cmd(cur, word);
	free(word);
	return (0);
}

int	handle_token_types(struct s_shell *shell, char **cmd, t_cmdnode **cur)
{
	if (**cmd == '<' && (*cmd)[1] == '<')
		return (handle_heredoc_token(shell, cmd, *cur));
	else if (**cmd == '<' || **cmd == '>')
		return (handle_redirection_token(shell, cmd, *cur));
	else if (**cmd == '|')
		return (handle_pipe_token(cmd, cur));
	else
		return (handle_word_token(shell, cmd, *cur));
}
