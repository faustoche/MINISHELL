/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:47:18 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/31 20:10:26 by faustoche        ###   ########.fr       */
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


t_env	*ft_cd(t_cmd *cmd, t_env *env_list)
{
	char	*home;
	//char	*pwd;
	char	*old_pwd;
	char	*new_dir;
	t_env	*new_env_list;

	new_dir = NULL;
	new_env_list = copy_env_list(env_list);
	home = find_var_value(new_env_list, "HOME");
	old_pwd = find_var_value(new_env_list, "PWD");
	if (cmd->nb_arg == 1)
		new_dir = home;
	else if (cmd->nb_arg == 2)
	{
		if (ft_strcmp(cmd->args[1], "-") == 0)
			new_dir = find_var_value(new_env_list, "OLDPWD");
		else if (ft_strcmp(cmd->args[1], "~") == 0 || ft_strcmp(cmd->args[1], "~/") == 0)
			new_dir = home;
		else
			new_dir = cmd->args[1];
	}
	else if (cmd->nb_arg > 2)
		printf(ERR_ARG, cmd->args[0]); // autre chose ??
	if (access(new_dir, F_OK) == -1)
	{
		perror("cd");
		free_env_list(new_env_list);
		return (NULL);
	}
	if (chdir(new_dir) == -1)
	{
		perror("cd");
		free_env_list(new_env_list);
		return (NULL);
	}
	new_dir = getcwd(NULL, 0);
	new_env_list = change_var_value(new_env_list, "OLDPWD", old_pwd);
	new_env_list = change_var_value(new_env_list, "PWD", new_dir);
	if (cmd->nb_arg == 2 && ft_strcmp(cmd->args[1], "-") == 0)
		ft_pwd();
	free(new_dir);
	free_env_list(env_list);
	return (new_env_list);
}
