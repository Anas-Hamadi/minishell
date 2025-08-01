/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:26:33 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 11:53:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void remove_empty_args(char **argv) {
    int i;
    int j;
    int has_non_empty_after_empty;

    // Check if we have non-empty args after empty ones
    // This indicates variable expansion like "$EMPTY echo hi"
    has_non_empty_after_empty = 0;
    i = 0;
    while (argv[i]) {
	if (argv[i][0] == '\0') {
	    // Found empty arg, check if there are non-empty args after
	    int k = i + 1;
	    while (argv[k]) {
		if (argv[k][0] != '\0') {
		    has_non_empty_after_empty = 1;
		    break;
		}
		k++;
	    }
	}
	i++;
    }

    // Only remove empty args if there are non-empty args after them
    if (!has_non_empty_after_empty)
	return;

    i = 0;
    j = 0;
    while (argv[i]) {
	if (argv[i][0] != '\0') {
	    if (i != j)
		argv[j] = argv[i];
	    j++;
	}
	i++;
    }
    argv[j] = NULL;
}

void handle_single_cmd(t_shell *shell) {
    if (shell->cmds->redirs) {
	if (handle_redirs(shell) < 0) {
	    shell->exit_code = 1;
	    return;
	}
    }
    if (shell->cmds->argv)
	remove_empty_args(shell->cmds->argv);
    if (!shell->cmds->argv || !shell->cmds->argv[0]) {
	shell->exit_code = 0;  // Truly empty command is successful no-op
	return;
    }
    // Note: empty string commands (from "") should go to builtin/exec check
    // where they will be handled as "command not found"
    if (!check_builtin(shell))
	check_exec(shell);
}
