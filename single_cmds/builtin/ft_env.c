#include "../../minishell.h"

void	ft_env(t_list *t_envp)
{
	t_list	*tmp;

	tmp = t_envp;
	while (tmp)
	{
		if (ft_strchr((char *)tmp->content, '='))
			printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}
