/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:40:47 by molamham          #+#    #+#             */
/*   Updated: 2025/07/29 16:56:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *get_target_path(char **argv) {
    char *home;

    if (argv[1])
	return (argv[1]);
    else {
	home = getenv("HOME");
	if (!home) {
	    ft_putendl_fd("cd: home not set\n", 2);
	    return (NULL);
	}
	return (home);
    }
}

void update_cd_env(t_list **envp, char *oldpwd) {
    char *newpwd;

    newpwd = getcwd(NULL, 0);
    if (!newpwd) {
	perror("getcwd");
	free(oldpwd);
	return;
    }
    update_env(envp, "OLDPWD", oldpwd);
    update_env(envp, "PWD", newpwd);
    free(newpwd);
}

void ft_cd(t_shell *shell) {
    char *path;
    char *oldpwd;

    path = get_target_path(shell->cmds->argv);
    if (!path) {
	shell->exit_code = 1;
	return;
    }
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd) {
	perror("getcwd");
	shell->exit_code = 1;
	return;
    }
    if (chdir(path) != 0) {
	perror("cd");
	free(oldpwd);
	shell->exit_code = 1;
	return;
    }
    update_cd_env(&shell->envp, oldpwd);
    free(oldpwd);
    shell->exit_code = 0;
}
