#include "../../minishell.h"

bool	ft_variable_exist(t_list *t_envp, char *var)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = t_envp;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, var, i))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

void	ft_add_ex_var(t_list **t_envp, char *var)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = *t_envp;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, var, i))
		{
			free(tmp->content);
			tmp->content = ft_strdup(var);
			return ;
		}
		tmp = tmp->next;
	}
}

void	print_content(char *str)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
	{
		printf("declare -x %s\n", str);
		return ;
	}
	key = ft_substr(str, 0, equal_sign - str);
	value = equal_sign + 1;
	printf("declare -x %s=\"%s\"\n", key, value);
	free(key);
}

void	ft_export(t_list **t_envp, char **input)
{
	int		y;
	int		i;
	char	**arr;

	i = 0;
	arr = sorted_env(*t_envp);
	if (!input[1])
	{
		while (arr[i])
			print_content(arr[i++]);
	}
	else
	{
		y = 1;
		while (input[y])
		{
			if (ft_variable_exist(*t_envp, input[y]))
				ft_add_ex_var(t_envp, input[y]);
			else
				ft_lstadd_back(t_envp, ft_lstnew(input[y]));
			y++;
		}
	}
}
