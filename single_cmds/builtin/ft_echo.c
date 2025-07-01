#include "../../minishell.h"
#include <stdio.h>

static bool	check_flag(char **args, int *i)
{
	int	j;

	if (!args[*i])
		return (true);
	j = 0;
	if (args[1][j++] != '-')
		return (true);
	while (args[1][j] == 'n') //we have a - so we go through the n's
		j++;
	if (args[1][j])    // we didnt reach the null terminator wich means -nnnnnnnnnp for example 
		return (true);
	(*i)++;
	return (false); //we reched the end wich means we have -nnnnnnnnn so we are not printing a new line .
}

static bool	handle_new_line(char **args, int *i)
{
	bool	print_new_line;
	int		j;

	print_new_line = check_flag(args, i);
	if (print_new_line == false) //-n kayna (ma gha ndirouch new line) , check il kano more -n's
	{
		while (args[*i])
		{
			j = 0;
			if (args[*i][0] != '-') // ma l9inach -nnn
				break ;
			j++;
			while (args[*i][j] == 'n')
				j++;
			if (args[*i][j]) // l9ina chi 7aja bla n -nnnx
				break ;
			(*i)++;
		}
	}
	return (print_new_line); // wach gha ndiro new line awla
}

void	ft_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = handle_new_line(args, &i); // false means no new line we pass the index i so the nnnnn get skipped 
	while (args[i])
	{
		ft_putstr(args[1], 1);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline == true)
		ft_putchar('\n', 1);
}

