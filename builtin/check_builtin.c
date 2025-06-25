#include "../minishell.h"

int	check_builtin(t_shell *shell)
{
	if (shell->input && !ft_strcmp(shell->s_input[0], "cd"))
		ft_cd(shell);
	else if (shell->input && !ft_strcmp(shell->s_input[0], "echo"))
		ft_echo(shell->s_input);
	else if (shell->input && !ft_strcmp(shell->s_input[0], "pwd"))
		ft_pwd(shell->s_input);
	else if (shell->input && !ft_strcmp(shell->s_input[0], "env"))
		ft_env(shell->t_envp);
	else if (shell->input && !ft_strcmp(shell->s_input[0], "exit"))
		ft_exit(shell->s_input);
	else if (shell->input && !ft_strcmp(shell->s_input[0], "unset"))
		ft_unset(shell->s_input, shell->t_envp);
	else if (shell->input && !ft_strcmp(shell->s_input[0], "export"))
		ft_export(&(shell->t_envp), shell->s_input);
	else
		return (0);
	return (1);
}
