/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:21:03 by molamham          #+#    #+#             */
/*   Updated: 2025/08/03 22:38:01 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strrchr_custom(const char *s, int c)
{
	size_t	i;

	i = ft_strlen(s);
	while (i > 0)
	{
		if (s[i - 1] == (char)c)
			return ((char *)(s + i - 1));
		i--;
	}
	if ((char)c == '\0')
		return ((char *)s + ft_strlen(s));
	return (NULL);
}

char	*ft_strdup_custom(const char *s)
{
	char	*arr;
	size_t	i;

	i = ft_strlen(s) + 1;
	arr = malloc(i);
	if (!arr)
		return (NULL);
	ft_memcpy(arr, s, i);
	arr[i - 1] = '\0';
	return (arr);
}

char	*find_env_value(t_list *envp, char *key)
{
	t_list	*tmp;
	char	*content;
	int		key_len;

	key_len = ft_strlen(key);
	tmp = envp;
	while (tmp)
	{
		content = (char *)tmp->content;
		if (!ft_strncmp(content, key, key_len) && content[key_len] == '=')
			return (content + key_len + 1);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_putendl_fd_custom(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}
