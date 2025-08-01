/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:15:05 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 20:26:42 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*safe_strcat_realloc(char **dest, size_t *dest_size, size_t *dest_len,
		const char *src)
{
	size_t	src_len;
	char	*new_dest;

	if (!dest || !*dest || !dest_size || !dest_len || !src)
		return (NULL);
	src_len = ft_strlen(src);
	while (*dest_len + src_len >= *dest_size - 1)
	{
		*dest_size *= 2;
		new_dest = malloc(*dest_size);
		if (!new_dest)
		{
			free(*dest);
			*dest = NULL;
			return (NULL);
		}
		ft_memcpy(new_dest, *dest, *dest_len);
		free(*dest);
		*dest = new_dest;
	}
	ft_memcpy(*dest + *dest_len, src, src_len);
	*dest_len += src_len;
	(*dest)[*dest_len] = '\0';
	return (*dest);
}

char	*safe_charcat_realloc(char **dest, size_t *dest_size, size_t *dest_len,
		char c)
{
	char	*new_dest;

	if (!dest || !*dest || !dest_size || !dest_len)
		return (NULL);
	if (*dest_len >= *dest_size - 1)
	{
		*dest_size *= 2;
		new_dest = malloc(*dest_size);
		if (!new_dest)
		{
			free(*dest);
			*dest = NULL;
			return (NULL);
		}
		ft_memcpy(new_dest, *dest, *dest_len);
		free(*dest);
		*dest = new_dest;
	}
	(*dest)[(*dest_len)++] = c;
	(*dest)[*dest_len] = '\0';
	return (*dest);
}

static char	*build_result_string(char *temp, int len, int is_negative)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(len + is_negative + 1);
	if (!result)
		return (NULL);
	i = 0;
	if (is_negative)
		result[i++] = '-';
	j = len - 1;
	while (j >= 0)
	{
		result[i++] = temp[j];
		j--;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_itoa_simple(int n)
{
	char	temp[16];
	int		len;
	int		is_negative;

	len = 0;
	is_negative = 0;
	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	while (n > 0)
	{
		temp[len++] = (n % 10) + '0';
		n /= 10;
	}
	return (build_result_string(temp, len, is_negative));
}
