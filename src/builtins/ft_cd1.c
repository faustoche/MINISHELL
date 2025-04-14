/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:47:18 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 07:52:13 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* chdir retour -1 si chemin invalide? ou si pas d'acces? */
	// récupérer la commande cd
	// chercher la variable HOME
	// sauvegarder l'ancien répertoire (OLDPWD)
	// exécuter cd et gérer les erreurs
	// mettre à jour OLDPWD si cd a reussi (dans les variables d'enviro)
	// recupérer le nouveau repertoire (PWD) et l'enregistrer
	// nettoyer la mémoire (getcwd utilise malloc) et retourner le statut

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
			return (1);
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
		return (cd_no_arg(home, new_dir));
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
	new_env = cd_update_env(cmd, new_env, old_pwd, new_dir);
	free_env_list(&env_list);
	return (new_env);
}

// t_env	*ft_cd(t_cmd *cmd, t_env *env_list)
// {
// 	char	*home;
// 	char	*old_pwd;
// 	char	new_dir[PATH_MAX];
// 	t_env	*new_env_list;

// 	ft_memset(new_dir, 0, sizeof(new_dir));
// 	new_env_list = copy_env_list(env_list);
// 	if (!new_env_list)
// 		return (env_list);
// 	home = find_var_value(new_env_list, "HOME");
// 	old_pwd = find_var_value(new_env_list, "PWD");
// 	if (cmd->nb_arg == 1)
// 	{
// 		if (!home || *home == '\0')
// 		{
// 			printf("minislay: cd: HOME not set\n");
// 			free_env_list(&new_env_list);
// 			return (env_list);
// 		}
// 		ft_strcpy(new_dir, home);
// 	}
// 	else if (cmd->nb_arg == 2)
// 	{
// 		if (!cmd->args[1] || cmd->args[1][0] == '\0')
//         {
//             if (!home || *home == '\0')
//             {
//                 printf("minislay: cd: HOME not set\n");
//                 free_env_list(&new_env_list);
//                 return (env_list);
//             }
//             ft_strcpy(new_dir, home);
//         }
// 		else if (ft_strcmp(cmd->args[1], "-") == 0)
// 			ft_strcpy(new_dir, find_var_value(new_env_list, "OLDPWD"));
// 		else if (ft_strcmp(cmd->args[1], "~") == 0
//			|| ft_strcmp(cmd->args[1], "~/") == 0)
// 			ft_strcpy(new_dir, home);
// 		else
// 			ft_strcpy(new_dir, cmd->args[1]);
// 	}
// 	else if (cmd->nb_arg > 2)
// 	{
// 		printf(ERR_ARG, cmd->args[0]);
// 		*(cmd->exit_status) = 1;
// 	}
// 	else if (access(new_dir, F_OK) == -1)
// 	{
// 		printf(ERR_DIR, cmd->args[1]);
// 		*(cmd->exit_status) = 1;
// 		free_env_list(&new_env_list);
// 		return (env_list);
// 	}
// 	if (chdir(new_dir) == -1)
// 	{
// 		perror("cd");
// 		*(cmd->exit_status) = 1;
// 		free_env_list(&new_env_list);
// 		return (env_list);
// 	}
// 	*(cmd->exit_status) = 0;
// 	if(!(getcwd(new_dir, sizeof(new_dir))))
// 		perror("error retrieving current directory");
// 	new_env_list = change_var_value(new_env_list, "OLDPWD", old_pwd);
// 	new_env_list = change_var_value(new_env_list, "PWD", new_dir);
// 	if (cmd->nb_arg == 2 && ft_strcmp(cmd->args[1], "-") == 0)
// 		ft_pwd_cd();
// 	free_env_list(&env_list);
// 	return (new_env_list);
// }
