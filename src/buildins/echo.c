/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:52:28 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/06 16:52:42 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(int	arg, char **list_args)
{
	int	i;

	i = 1;
	if (arg > 1)
	{
		while (i < arg)
		{
			printf("%s", list_args[i]);
			if (i < arg - 1)
				printf(" ");
			i++;
		}
		printf("\n");
	}
}
