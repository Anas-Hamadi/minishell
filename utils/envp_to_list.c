#include "../minishell.h"


void update_env(t_list **envp, char *key, char *value)
{
	char *new_str;
	char *tmp_str;
	t_list *tmp;
	int key_len;

	tmp = *envp;
	key_len = ft_strlen(key);
	tmp_str = ft_strjoin(key, "=");
	new_str = ft_strjoin(tmp_str, value);
	printf("Allocated tmp_str: %p\n", (void *)tmp_str);
	printf("Allocated new_str: %p\n", (void *)new_str);
	free(tmp_str);
	while (tmp)
	{

		if (!ft_strncmp((char *)tmp->content, key, key_len) && ((char *)tmp->content)[key_len] == '=')
		{
			free(tmp->content);
			tmp->content = new_str;
			return;
		}
		tmp = tmp->next;
	}
	tmp = ft_lstnew(new_str);
	if (!tmp) // still leaks might remove it later
	{
		free(new_str);
		return;
	}
	ft_lstadd_back(envp, ft_lstnew(ft_strdup(new_str)));
	free(new_str);
}

// t_list *envp_to_list(char **envp)
// {
// 	int i;
// 	t_list *t_envp;

// 	i = 0;
// 	t_envp = NULL;
// 	while (envp[i])
// 	{
// 		ft_lstadd_back(&t_envp, ft_lstnew(ft_strdup(envp[i])));
// 		i++;
// 	}
// 	return (t_envp);
// }

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