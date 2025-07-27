/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:45:03 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 15:46:19 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_pwd(char **s_input)
{
	char	*cwd;

	(void) s_input;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return ;
	}
	else if (s_input[0] && s_input[1])
	{
		printf("minishell: pwd: too many arguments\n");
		ft_free(s_input);
		return ;
	}
	printf("%s\n", cwd);
}
