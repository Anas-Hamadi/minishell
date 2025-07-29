/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:45:03 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:56:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_pwd(t_shell *shell) {
    char *cwd;
    char **s_input;

    s_input = shell->cmds->argv;
    cwd = getcwd(NULL, 0);
    if (!cwd) {
	perror("pwd");
	shell->exit_code = 1;
	return;
    } else if (s_input[0] && s_input[1]) {
	ft_putstr_fd("minishell: pwd: too many arguments\n", 2);
	free(cwd);
	shell->exit_code = 1;
	return;
    }
    ft_putstr_fd(cwd, 1);
    ft_putstr_fd("\n", 1);
    free(cwd);
    shell->exit_code = 0;
}
