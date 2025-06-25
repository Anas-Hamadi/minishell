#include "../minishell.h"

t_list	*envp_to_list(char **envp)
{
	int		i;
	t_list	*t_envp;

	i = 0;
	t_envp = NULL;
	while (envp[i]) 
	{
		ft_lstadd_back(&t_envp, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
	return (t_envp);
}
