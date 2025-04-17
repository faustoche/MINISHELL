/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:47:18 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/17 09:50:47 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd_cd(void)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
}

static int	cd_one_arg(t_cmd *cmd, t_env *env_list, char *home, char *new_dir)
{
	if (!cmd->args[1] || cmd->args[1][0] == '\0')
	{
		if (cd_no_home(home))
		{
			*(cmd->exit_status) = 1;
			return (1);
		}
		ft_strcpy(new_dir, home);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		ft_strcpy(new_dir, find_var_value(env_list, "OLDPWD"));
	else if (ft_strcmp(cmd->args[1], "~") == 0
		|| ft_strcmp(cmd->args[1], "~/") == 0)
		ft_strcpy(new_dir, home);
	else
		ft_strcpy(new_dir, cmd->args[1]);
	return (0);
}

static int	cd_many_args(t_cmd *cmd)
{
	printf(ERR_ARG, cmd->args[0]);
	*(cmd->exit_status) = 1;
	return (1);
}

static int	cd_args(t_cmd *cmd, t_env *env_list, char *home, char *new_dir)
{
	if (cmd->nb_arg == 1)
	{
		if (cd_no_arg(home, new_dir))
		{
			*(cmd->exit_status) = 1;
			return (1);
		}
	}
	else if (cmd->nb_arg == 2)
		return (cd_one_arg(cmd, env_list, home, new_dir));
	else if (cmd->nb_arg > 2)
		return (cd_many_args(cmd));
	return (0);
}

t_env	*ft_cd(t_cmd *cmd, t_env *env_list)
{
	char	*home;
	char	*old_pwd;
	char	new_dir[PATH_MAX];
	t_env	*new_env;

	ft_memset(new_dir, 0, sizeof(new_dir));
	new_env = copy_env_list(env_list);
	if (!new_env)
		return (env_list);
	home = find_var_value(new_env, "HOME");
	old_pwd = find_var_value(new_env, "PWD");
	if (cd_args(cmd, new_env, home, new_dir))
	{
		free_env_list(&new_env);
		return (env_list);
	}
	if (cd_errors(cmd, new_dir, &new_env))
		return (env_list);
	new_env = cd_upda_env(cmd, new_env, old_pwd, new_dir);
	free_env_list(&env_list);
	return (new_env);
}
