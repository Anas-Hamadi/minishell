#include "../minishell.h"

void	ft_lstfree(t_list *lst)
{
	free(lst->content);
	free(lst);
}

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

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = ft_strlen((char *)s);
	while (i > 0)
	{
		if (s[i - 1] == (char)c)
			return ((char *)(s + i - 1));
		i--;
	}
	if ((char)c == '\0')
		return ((char *)s + ft_strlen((char *)s));
	return (0);
}

char	*ft_strdup(const char *s)
{
	char	*arr;
	size_t	i;

	i = ft_strlen((char *)s) + 1;
	arr = malloc(i);
	if (!arr)
		return (NULL);
	ft_memcpy(arr, s, i);
	arr[i - 1] = '\0';
	return (arr);
}

void	ft_putendl_fd(char *s, int fd)
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
