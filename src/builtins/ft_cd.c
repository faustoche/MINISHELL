/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:47:18 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/27 21:13:59 by faustoche        ###   ########.fr       */
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

static char	*find_var_value(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
	//si cmd->args[1] est .. , on remplace env_list->value correspondant a OLDPWD par repertoire parent dans env_list
t_env	*change_var_value(t_env *env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*head;

	head = env_list;
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			printf("change_var : [%s=%s]\n\n", current->name, current->value);
			break ;
		}
		current = current->next;
	}
	return (head);
}

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
	//pwd = find_var_value(new_env_list, "PWD");
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
		// else
		// {
		// 	new_dir = build_pathname(pwd, cmd->args[1]);// si go repertoire enfant et si chemin relatif
		// 	printf("%s\n\n", new_dir);
		// } 
		// 	//a faire pour chemin absolu
		// 	//a faire pour ..
	}
	else if (cmd->nb_arg > 2)
		printf(ERR_ARG, cmd->args[0]); // autre chose ??
	if (access(new_dir, F_OK) == -1) // verifie que chemin existe
	{
		perror("cd");
		return (NULL);
	}
	if (chdir(new_dir) == -1) // change pour le chemin de new_dir
	{
		perror("cd");
		return (NULL);
	}
	new_env_list = change_var_value(new_env_list, "OLDPWD", old_pwd); // changer la valeur de OLDPWD dans env_list
	new_env_list = change_var_value(new_env_list, "PWD", new_dir); //changer la valeur de PWD dans env_list
	if (cmd->nb_arg == 2 && ft_strcmp(cmd->args[1], "-") == 0)
		ft_pwd();
	return (new_env_list);
}
