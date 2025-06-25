#include "minishell.h"
#include <readline/history.h>
#include <unistd.h>

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_shell	shell;

	shell.t_envp = envp_to_list(envp);
	while (1)
	{
		shell.input = readline("\033[34mMinishel$ \033[0m");
		if (shell.input)
		{
			if (ft_strchr(shell.input, '|'))
			{
				shell.s_input = ft_split(shell.input, '|');
				handle_pipes(&shell);
			}
			else 
			{
				shell.s_input = ft_split(shell.input, ' ');
				if (!check_builtin(&shell))
					check_exec(&shell);
			}
			add_history(shell.input);
		}
		if (shell.s_input)
			ft_free(shell.s_input);
	}
	ft_free(shell.s_input);
	free(shell.input);
}

