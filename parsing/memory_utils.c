/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:15:05 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 15:15:06 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/* Safe string concatenation with reallocation */
char	*safe_strcat_realloc(char **dest, size_t *dest_size, size_t *dest_len,
		const char *src)
{
	size_t	src_len;
	char	*new_dest;

	if (!dest || !*dest || !dest_size || !dest_len || !src)
		return (NULL);
	src_len = strlen(src);
	/* Check if we need to expand */
	while (*dest_len + src_len >= *dest_size - 1)
	{
		*dest_size *= 2;
		new_dest = realloc(*dest, *dest_size);
		if (!new_dest)
		{
			free(*dest);
			*dest = NULL;
			return (NULL);
		}
		*dest = new_dest;
	}
	strcpy(*dest + *dest_len, src);
	*dest_len += src_len;
	return (*dest);
}

/* Safe character append with reallocation */
char	*safe_charcat_realloc(char **dest, size_t *dest_size, size_t *dest_len,
		char c)
{
	char	*new_dest;

	if (!dest || !*dest || !dest_size || !dest_len)
		return (NULL);
	/* Check if we need to expand */
	if (*dest_len >= *dest_size - 1)
	{
		*dest_size *= 2;
		new_dest = realloc(*dest, *dest_size);
		if (!new_dest)
		{
			free(*dest);
			*dest = NULL;
			return (NULL);
		}
		*dest = new_dest;
	}
	(*dest)[(*dest_len)++] = c;
	(*dest)[*dest_len] = '\0';
	return (*dest);
}

/* Convert integer to string without snprintf */
char	*ft_itoa_simple(int n)
{
	char	*result;
	char	temp[16];
	int		len;
	int		is_negative;
	int		i;

	len = 0;
	is_negative = 0;
	if (n == 0)
		return (strdup("0"));
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	/* Convert to string in reverse */
	while (n > 0)
	{
		temp[len++] = (n % 10) + '0';
		n /= 10;
	}
	result = malloc(len + is_negative + 1);
	if (!result)
		return (NULL);
	i = 0;
	if (is_negative)
		result[i++] = '-';
	/* Reverse the digits */
	for (int j = len - 1; j >= 0; j--)
		result[i++] = temp[j];
	result[i] = '\0';
	return (result);
}
