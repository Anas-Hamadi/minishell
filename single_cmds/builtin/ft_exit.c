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
			return (1);
		i++;
	}
	return (0);
}

void	ft_exit_error(char **s_input)
{
	if (ft_isvalid_arg(s_input[1]) == 1)
	{
		printf("minishell: exit: %s: numeric argument required\n", s_input[1]);
		ft_free(s_input);
		exit(255);
	}
	else if (s_input[1] && s_input[2])
		printf("minishell: exit: too many arguments\n");
}

void	ft_exit(char **s_input)
{
	long long	num;

	num = 0;
	if (!s_input[1])
	{
		ft_free(s_input);
		exit(0);
	}
	else if (ft_isvalid_arg(s_input[1]) == 0 && !s_input[2])
	{
		num = ft_atoi(s_input[1]);
		if (num < INT_MAX && num > INT_MIN)
		{
			ft_free(s_input);
			exit(num);
		}
	}
	else
		ft_exit_error(s_input);
}
