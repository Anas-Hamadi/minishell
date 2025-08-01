/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:01:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:32:15 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parse.h"

char	*get_env_value(struct s_shell *shell, const char *key)
{
	t_list	*tmp;
	char	*content;
	size_t	key_len;

	if (!key || !shell || !shell->envp)
		return (NULL);
	key_len = ft_strlen(key);
	tmp = shell->envp;
	while (tmp)
	{
		content = (char *)tmp->content;
		if (content && ft_strncmp(content, key, key_len) == 0
			&& content[key_len] == '=')
		{
			return (ft_strdup(content + key_len + 1));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	get_last_exit_status(struct s_shell *shell)
{
	return (shell->exit_code);
}
