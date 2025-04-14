/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:58:50 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 13:22:13 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtins(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static void	builtins_exec2(t_cmd *cmd, t_env **env_list)
{
	size_t	i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		*env_list = ft_cd(cmd, *env_list);
	if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env(*env_list);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		ft_exit(cmd);
	if (ft_strcmp(cmd->args[0], "unset") == 0 && cmd->args[1])
	{
		if (cmd->nb_arg > 1)
		{
			i = 1;
			while (i < cmd->nb_arg)
			{
				*env_list = ft_unset(*env_list, cmd->args[i]);
				i++;
			}
		}
	}
}

void	builtins_execution(t_cmd *cmd, t_env **env)
{
	size_t	i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		if (cmd->nb_arg == 1)
			*env = ft_export(*env, NULL, cmd->exit_status);
		else
		{
			i = 1;
			while (i < cmd->nb_arg)
			{
				*env = ft_export(*env, cmd->args[i], cmd->exit_status);
				i++;
			}
		}
	}
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd(cmd);
	builtins_exec2(cmd, env);
}
