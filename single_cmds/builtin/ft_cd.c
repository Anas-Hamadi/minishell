#include "../../minishell.h"
#include <stdio.h>
#include <unistd.h>
//
// void	ft_cd(t_shell *shell)
// {
// 	char	*path;
//
// 	path = shell->s_input[1];
// 	if (!path)
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 		{
// 			printf("cd : HOME not set\n");
// 			return ;
// 		}
// 	}
// 	if (chdir(path) != 0)
// 		perror("cd");
// }

void	ft_cd(t_cmdnode *cmd_list)
{
	char	*path;

	path = cmd_list->argv[1];
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr("cd : HOME not set\n", 2);
			return ;
		}
	}
	if (chdir(path) != 0)
		perror("cd");
}
