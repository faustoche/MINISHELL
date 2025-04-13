/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:39:10 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/13 20:45:30 by faustoche        ###   ########.fr       */
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

t_env	*cd_update_env(t_cmd *cmd, t_env *new_env_list, char *old_pwd, char *new_dir)
{
	*(cmd->exit_status) = 0;
	if (!(getcwd(new_dir, PATH_MAX)))
		perror("error retrieving current directory");
	new_env_list = change_var_value(new_env_list, "OLDPWD", old_pwd);
	new_env_list = change_var_value(new_env_list, "PWD", new_dir);
	if (cmd->nb_arg == 2 && ft_strcmp(cmd->args[1], "-") == 0)
		ft_pwd_cd();
	return (new_env_list);
}
