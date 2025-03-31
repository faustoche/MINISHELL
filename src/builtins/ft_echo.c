/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:52:28 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/31 15:12:11 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Ajouter un clean propre a la fin */

void	ft_echo(t_cmd *cmd)
{
	size_t	i;
	size_t	j;
	int		newline;

	newline = 1;
	i = 1;
	while (i < cmd->nb_arg && cmd->args[i][0] == '-'
		&& cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
	{
		j = 1;
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] != '\0')
			break ;
		newline = 0;
		i++;
	}
	while (i < cmd->nb_arg)
	{
		printf("%s", cmd->args[i]);
		if (i < cmd->nb_arg - 1)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
