#include "../minishell.h"

void free_argv(char **argv)
{
    int i = 0;
    if (!argv)
        return;
    while (argv[i])
        free(argv[i++]);
    free(argv);
}

void free_redirs(t_redir *redir)
{
    t_redir *tmp;
    while (redir)
    {
        tmp = redir->next;
        if (redir->filename)
            free(redir->filename);
        free(redir);
        redir = tmp;
    }
}

void free_cmds_list(t_cmdnode *cmd_list)
{
    t_cmdnode *tmp;
    while (cmd_list)
    {
        tmp = cmd_list->next;
        free_argv(cmd_list->argv);
        free_redirs(cmd_list->redirs);
        free(cmd_list);
        cmd_list = tmp;
    }
}

void free_shell(t_shell *shell)
{
    printf("shell\n");
    if (!shell)
        return;
    if (shell->input)
        free(shell->input);
    if (shell->cmds)
        free_cmds_list(shell->cmds);
    if (shell->envp)
        ft_lstclear(&shell->envp, free);
}

// void    free_shell(t_shell *shell)
// {
//     printf("=== FREEING SHELL ===\n");
//     if (!shell)
//         return;
    
//     // Print environment before freeing
//     printf("Environment before cleanup:\n");
//     t_list *env = shell->envp;
//     while (env)
//     {
//         printf("  %s\n", (char *)env->content);
//         env = env->next;
//     }
    
//     if (shell->input)
//         free(shell->input);
//     if (shell->cmds)
//         free_cmds_list(shell->cmds);
//     if (shell->envp)
//     {
//         printf("Freeing env list...\n");
//         ft_lstclear(&shell->envp, free);
//         printf("Env list after clear: %p\n", (void *)shell->envp);
//     }
//     printf("=== SHELL FREED ===\n");
// }