#include "../minishell.h"
#include <unistd.h>

void	ft_putchar(int c, int fd)
{
	write(1, &c, fd);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
		ft_putchar(str[i++], fd);
}
