#include "../../minishell.h"

int check_builtin(t_shell *shell)
{
	if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "cd"))
		ft_cd(shell);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "echo"))
		ft_echo(shell->cmds->argv);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "pwd"))
		ft_pwd(shell->cmds->argv);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "env"))
		ft_env(shell->envp);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "exit"))
		ft_exit(shell->cmds->argv); // still need re-do ???
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "unset"))
		ft_unset(shell->cmds->argv, shell->envp);
	else if (shell->cmds->argv[0] && !ft_strcmp(shell->cmds->argv[0], "export"))
		ft_export(&shell->envp, shell->cmds->argv);
	else
		return (0);
	return (1);
}
