/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 22:15:00 by ahamadi          #+#    #+#             */
/*   Updated: 2025/08/03 22:15:00 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_target_path(char **argv)
{
	char	*home;

	if (argv[1])
		return (argv[1]);
	else
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putendl_fd("cd: home not set\n", 2);
			return (NULL);
		}
		return (home);
	}
}
