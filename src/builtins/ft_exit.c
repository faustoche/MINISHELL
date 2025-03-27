/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:14:36 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/27 11:08:52 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
- Why atoi ? Exit codes in linux are limited to 8 bits (0-255)
- If the user put a value that's out of range (ex: exit 1000)
- Shell has to bring back the value in the 0-255 range
- 1000 % 256 = 232 thus exit 1000 = exit 232
*/

/* Ajouter une fonction a la toute fin pour exit le programme proprement */

void	ft_exit(t_cmd *cmd)
{
	int		exit_code;

	printf("Adieu 💀\n");
	exit_code = 0;
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]))
		{
			printf("minislay : exit: %s: numbers required\n", cmd->args[1]);
			exit(255);
		}
		else if (cmd->args[2])
		{
			printf("minislay : exit: too many arguments\n");
			return ;
		}
		else
			exit_code = ft_atoi(cmd->args[1]) % 256;
	}
	exit(exit_code);
}
