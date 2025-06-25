#include "../minishell.h"

void	*ft_memcpy(void	*dest, const void *src, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;
	size_t			i;

	i = 0;
	s1 = (unsigned char *)dest;
	s2 = (unsigned char *)src;
	if (dest != src)
	{
		while (i < n)
		{
			s1[i] = s2[i];
			i++;
		}
	}
	return (dest);
}
