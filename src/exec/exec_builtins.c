/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:58:50 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/26 18:06:39 by fcrocq           ###   ########.fr       */
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

int builtins_execution(t_cmd *cmd, t_env **env_list)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd);
		return (0);
	}
	// if (ft_strcmp(cmd->args[0], "cd") == 0)
	// {
	//     ft_cd(cmd);
	//     return (0);
	// }
	// if (ft_strcmp(cmd->args[0], "pwd") == 0)
	//     return ft_pwd();
	if (ft_strcmp(cmd->args[0], "export") == 0 && cmd->args[1])
	{
		*env_list = ft_export(*env_list, cmd->args[1]);
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "unset") == 0 && cmd->args[1])
	{
		*env_list = ft_unset(*env_list, cmd->args[1]);
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		ft_env(*env_list);
		return (0);
	}
	// if (ft_strcmp(cmd->args[0], "exit") == 0)
	//     exit(0);
	return (0);
}
