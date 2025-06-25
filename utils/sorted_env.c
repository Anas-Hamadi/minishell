#include "../minishell.h"

void	swap_value(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

int	var_name_comp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] != '=' && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s1[i] == '=' && s2[i] != '=')
		return (-1);
	if (s1[i] != '=' && s2[i] == '=')
		return (1);
	return (0);
}

char	**sorted_env(t_list *env)
{
	int	i;
	char	**arr;
	t_list	*tmp;

	tmp = env;
	i = ft_lstsize(env);
	arr = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	i = 0;
	while (arr[i] && arr[i + 1])
	{
		if (var_name_comp(arr[i], arr[i + 1]) > 0)
		{
			swap_value(&arr[i], &arr[i + 1]);
			i = -1;
		}
		i++;
	}
	return (arr);
}

