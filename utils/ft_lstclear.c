#include "../minishell.h"

void ft_lstclear(t_list **lst, void (*del)(void *))
{
    t_list *tmp;
    while (*lst)
    {
        tmp = (*lst)->next;
        if (del)
            del((*lst)->content);
        free(*lst);
        *lst = tmp;
    }
    *lst = NULL;
}

// void ft_lstclear(t_list **lst, void (*del)(void *))
// {
//     t_list *tmp;

//     printf("CLEARING LIST OF %d NODES\n", ft_lstsize(*lst));
//     while (*lst)
//     {
//         tmp = (*lst)->next;
//         printf("  Freeing node %p (content: %p)\n", *lst, (*lst)->content);
//         del((*lst)->content);
//         free(*lst);
//         *lst = tmp;
//     }
// }