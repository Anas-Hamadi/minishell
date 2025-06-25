#include "../minishell.h"
#include <stdio.h>

void	ft_cd(t_shell *shell)
{
	char	*path;

	path = shell->s_input[1];
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd : HOME not set\n");
			return ;
		}
	}
	if (chdir(path) != 0)
		perror("cd");
}
