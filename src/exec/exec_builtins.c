/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:58:50 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/21 12:46:57 by faustoche        ###   ########.fr       */
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

int	builtins_execution(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
		return (1);
	}
	// if (ft_strcmp(cmd->args[0], "pwd") == 0)
	// {
	// 	ft_echo(cmd);
	// 	return (1);
	// }
	builtins_execution2(cmd);
	return (0);
}

int	builtins_execution2(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	// if (ft_strcmp(cmd->args[0], "unset") == 0)
	// {
	// 	ft_echo(cmd);
	// 	return (1);
	// }
	// if (ft_strcmp(cmd->args[0], "export") == 0)
	// {
	// 	ft_echo(cmd);
	// 	return (1);
	// }
	// if (ft_strcmp(cmd->args[0], "env") == 0)
	// {
	// 	ft_env(cmd);
	// 	return (1);
	// }
	if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd);
		return (1);
	}
	return (0);
}
