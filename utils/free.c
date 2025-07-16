#include "../minishell.h"

void	free_argv(char **argv)
{
	int	i = 0;

	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_env_list(t_list *envp)
{
	t_list *tmp;

	while (envp)
	{
		tmp = envp->next;
		if (envp->content)
			free(envp->content);
		free(envp);
		envp = tmp;
	}
}

void	free_cmd_list(t_cmdnode *cmd_list)
{
	t_cmdnode *tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_argv(cmd_list->argv);
		free_redirs(cmd_list->redirs);
		free_env_list(cmd_list->envp);
		free(cmd_list);
		cmd_list = tmp;
	}
}

