#include "../minishell.h"


void update_env(t_list **envp, char *key, char *value)
{
	char *new_str;
	char *tmp_str;
	t_list *tmp;

	tmp = *envp;
	tmp_str = ft_strjoin(key, "=");
	new_str = ft_strjoin(tmp_str, value);
	free(tmp_str);
	while (tmp)
	{
		if (!ft_strncmp((char *)tmp->content, key, ft_strlen(key)) && ((char *)tmp->content)[ft_strlen(key)] == '=')
		{
			free(tmp->content);
			tmp->content = new_str;
			return;
		}
		tmp = tmp->next;
	}
	ft_lstadd_back(envp, ft_lstnew(ft_strdup(new_str)));
	free(new_str);
}

t_list *envp_to_list(char **envp)
{
	int i;
	t_list *t_envp;
	t_list *new;
	char *dup;

	i = 0;
	t_envp = NULL;
	while (envp[i])
	{
		dup = ft_strdup(envp[i]);
		if (!dup)
			return (ft_lstclear(&t_envp, free), NULL);
		new = ft_lstnew(dup);
		if (!new)
		{
			free(dup);
			return (ft_lstclear(&t_envp, free), NULL);
		}
		ft_lstadd_back(&t_envp, new);
		i++;
	}
	return (t_envp);
}