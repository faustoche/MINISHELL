/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:39:10 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/15 07:52:15 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_errors(t_cmd *cmd, char *dir, t_env **new_env)
{
	if (access(dir, F_OK) == -1)
	{
		printf(ERR_DIR, cmd->args[1]);
		*(cmd->exit_status) = 1;
		free_env_list(new_env);
		return (1);
	}
	if (chdir(dir) == -1)
	{
		perror("cd");
		*(cmd->exit_status) = 1;
		free_env_list(new_env);
		return (1);
	}
	return (0);
}

int	cd_no_home(char *home)
{
	if (!home || *home == '\0')
	{
		printf("minislay: cd: HOME not set\n");
		return (1);
	}
	return (0);
}

int	cd_no_arg(char *home, char *new_dir)
{
	if (cd_no_home(home))
		return (1);
	ft_strcpy(new_dir, home);
	return (0);
}

t_env	*cd_upda_env(t_cmd *cmd, t_env *new_env, char *old_pwd, char *new_dir)
{
	*(cmd->exit_status) = 0;
	if (!(getcwd(new_dir, PATH_MAX)))
		perror("error retrieving current directory");
	new_env = change_var_value(new_env, "OLDPWD", old_pwd);
	new_env = change_var_value(new_env, "PWD", new_dir);
	if (cmd->nb_arg == 2 && ft_strcmp(cmd->args[1], "-") == 0)
		ft_pwd_cd();
	return (new_env);
}
