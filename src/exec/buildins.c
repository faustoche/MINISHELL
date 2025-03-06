/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:58:50 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/06 16:49:24 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtins(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (-1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (-1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (-1);
	if (ft_strcmp(cmd, "unset") == 0)
			return (-1);
	if (ft_strcmp(cmd, "export") == 0)
			return (-1);
	if (ft_strcmp(cmd, "env") == 0)
		return (-1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (-1);
	return (0);
}

int	buildins_execution(char **arg)
{
	
}
