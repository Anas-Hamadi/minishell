#include "../minishell.h"
#include <unistd.h>

void handle_single_cmd(t_shell *shell)
{
	if (shell->cmds->redirs)
		handle_redirs(shell);
	if (!shell->cmds->argv || !shell->cmds->argv[0])
		return;
	if (!check_builtin(shell))
		check_exec(shell);
}
