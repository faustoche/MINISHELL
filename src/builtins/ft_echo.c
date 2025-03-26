/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:52:28 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/20 09:39:09 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_cmd *cmd)
{
	size_t	i;
	int		newline;

	newline = 1;
	i = 1;
	if (cmd->nb_arg > 1 && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	else
		i = 1;
	while (i < cmd->nb_arg)
	{
		printf("%s", cmd->args[i]);
		if (i < cmd->nb_arg - 1)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	// ici ajouter un clean
}