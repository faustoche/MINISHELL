/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:14:36 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/11 14:46:32 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
- Why atoi ? Exit codes in linux are limited to 8 bits (0-255)
- If the user put a value that's out of range (ex: exit 1000)
- Shell has to bring back the value in the 0-255 range
- 1000 % 256 = 232 thus exit 1000 = exit 232
*/

/* exit -> quitte avec le dernier code connu */
/* exit 42 -> quitte avec le code 42 */
/* exit fff -> quitte avec le code 2 car ce n'est pas un nombre */
/* exit 1 2 -> trop d'arguments */
/* exit -9223372036854775807 (LONG LONG INT MAX) -> quitte avec le code 2 car ce n'est pas un nombre*/

void	ft_exit(t_cmd *cmd)
{
	long long		exit_code;
	
	exit_code = 0;
	printf("Adieu 💀\n");
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]))
		{
			printf("minislay : exit: %s: numeric argument required\n", cmd->args[1]);
			exit(2);
		}
		else if (cmd->args[2])
		{
			printf("minislay : exit: too many arguments\n");
			return ;
		}
		else
			exit_code = atoll(cmd->args[1]) % 256;
		if (exit_code < 0)
			exit_code += 256;
	}
	// else
	// 	exit_code = cmd->exit_status;
	quit_minislay(NULL, cmd, NULL, NULL);
	exit(exit_code);
}
