/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:03:10 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:29:27 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

char	*handle_word(struct s_shell *shell, char **cmd, bool in_del,
		bool *expand_in_hd)
{
	char			*buffer;
	size_t			buffer_size;
	size_t			buffer_len;
	t_word_buffer	wb;

	(void)expand_in_hd;
	buffer_len = 0;
	buffer_size = 256;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	wb = (t_word_buffer){&buffer, &buffer_size, &buffer_len, NULL};
	while (**cmd && !is_metachar(**cmd))
	{
		if (!process_word_char(shell, cmd, in_del, &wb))
		{
			free(buffer);
			return (NULL);
		}
	}
	return (buffer);
}
