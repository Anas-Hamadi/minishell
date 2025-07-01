#include "../minishell.h"
#include <unistd.h>

void	handle_single_cmd(t_cmdnode *cmd_list)
{
	if (!check_builtin(cmd_list))
		check_exec(cmd_list);
}
