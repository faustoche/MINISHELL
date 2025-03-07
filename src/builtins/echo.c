/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:52:28 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/07 15:20:45 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	echo_check(int arg, char **list_args)
{
	if (arg > 1 && ft_strcmp(list_args[1], "-n") == 0)
		return (0);
	return (1);
}

void	ft_echo(int	arg)
{
	char	**list_args;
	int		i;
	int		newline;

	
	i = 1;
	newline = echo_check(arg, list_args);
	if (newline == 0)
		i = 2;
	else
		i = 1;
	while (i < arg)
	{
		printf("%s", list_args[i]);
		if (i < arg - 1)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
