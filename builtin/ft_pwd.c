#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

void	ft_pwd(char **s_input)
{
	(void) s_input;
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return ;
	}
	else if (s_input[0] && s_input[1])
	{
		printf("minishell: pwd: too many arguments\n");
		ft_free(s_input);
		return ;
	}
	printf("%s\n", cwd);
}
