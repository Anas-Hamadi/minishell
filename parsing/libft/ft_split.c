/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:01:36 by ahamadi           #+#    #+#             */
/*   Updated: 2024/11/24 15:01:37 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_tokens(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static void	*free_split(char **split, int j)
{
	while (j >= 0)
	{
		free(split[j]);
		j--;
	}
	free(split);
	return (NULL);
}

static char	*copy_token(char const *s, int start, int end)
{
	char	*token;
	int		i;

	token = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (start < end)
		token[i++] = s[start++];
	token[i] = '\0';
	return (token);
}

static int	add_token(char **split, char const *s, char c, int *j)
{
	int	start;
	int	end;

	end = 0;
	while (s[end])
	{
		while (s[end] == c && s[end])
			end++;
		start = end;
		while (s[end] != c && s[end])
			end++;
		if (end > start)
		{
			split[*j] = copy_token(s, start, end);
			if (!split[*j])
				return (0);
			(*j)++;
		}
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		j;

	if (!s)
		return (NULL);
	split = (char **)malloc((count_tokens(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	j = 0;
	if (!add_token(split, s, c, &j))
		return (free_split(split, j - 1));
	split[j] = NULL;
	return (split);
}
