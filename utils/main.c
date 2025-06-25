#include "minishell.h"
#include <readline/history.h>
#include <unistd.h>

void	check_pipe(t_shell *shell)
{
	// char *argv;
	// char *tmp;
	//char **args;
	int	i = 0;
	while (shell->s_input[i])
	{
		char **curent_command = ft_split(shell->s_input[i], ' ');
		//sawb pipe (pipe[fd])
		//forki
		//if child
			//change input o outpu (dup2)
			//execve 3la curent_command
		//if parent
			//savi pipe fd;
			//savi pid
		i++;
	}
	// mni tssali while loop wait for childs
	




	//args = malloc (sizeof(char *) * (i + 1));
	//int pipes = i - 1;
		// i = 0;
	// while (shell->s_input[i])
	// {
	// 	tmp = shell->s_input[i];
	// 	argv = ft_strjoin(argv, tmp);
	// 	i++;
	// }
	// //args[i] = NULL;
	// i = 0;
	// //while (argv)
	// //	printf("%s\n", args[i++]);
	// printf("%s\n", argv);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_shell	shell;

	shell.t_envp = envp_to_list(envp);
	while (1)
	{
		shell.input = readline("\033[34mMinishel$ \033[0m");
		shell.s_input = ft_split(shell.input, '|');
		if (shell.input)
		{
			check_pipe(&shell);
			// if (!check_builtin(&shell))
			// 	check_exec(&shell);
			add_history(shell.input);
		}
		ft_free(shell.s_input);
	}
	ft_free(shell.s_input);
	free(shell.input);
}

