#include "../minishell.h"

void	ft_lstadd_back(t_list **lst, t_list *t_new)
{
	t_list	*last;

	if (!lst || !t_new)
		return ;
	if (!*lst)
		*lst = t_new;
	else
	{
		last = ft_lstlast(*lst);
		last->next = t_new;
	}
}
