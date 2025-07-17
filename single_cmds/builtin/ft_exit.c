#include "../../minishell.h"

int	ft_isvalid_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

static void	ft_exit_error(t_shell *shell, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_shell(shell);
	exit(255);
}

void	ft_exit(t_shell *shell)
{
	char		**argv;
	long long	exit_code;

	argv = shell->cmds->argv;
	exit_code = 0;
	ft_putendl_fd("exit", 1);
	if (argv[1] && !ft_isvalid_arg(argv[1]))
		ft_exit_error(shell, argv[1]);
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return ;
	}
	if (argv[1])
		exit_code = ft_atoi(argv[1]);
	free_shell(shell);
	exit(exit_code);
}

