/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 08:47:18 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/18 11:53:37 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* chdir retour -1 si chemin invalide? ou si pas d'acces? */

	// récupérer la commande cd
	// chercher la variable HOME
	// sauvegarder l'ancien répertoire (OLDPWD)
	// exécuter cd et gérer les erreurs
	// mettre à jour OLDPWD si cd a reussi
	// recupérer le nouveau repertoire (PWD) et l'enregistrer
	// nettoyer la mémoire (getcwd utilise malloc) et retourner le statut

int ft_cd(t_cmd *cmd)
{
	char	*pwd;
	char	*home;
	int		res;

	pwd = getcwd(NULL, 0);
	home = getenv("HOME");
	printf("pwd before chdir: %s\n", pwd);

	if (cmd->nb_arg > 2)
		printf(ERR_ARG, cmd->args[0]);
	else if (cmd->nb_arg == 1)
	{
		res = chdir(home);
		if (res == -1)
			perror("cd");
		//si a fonctionne, met a jour oldpwd
	}
	else if (cmd->nb_arg == 2)
	{
		res = chdir(cmd->args[1]);
		if (res == -1)
			perror("cd");
		//si a fonctionne, met a jour oldpwd
	}
	pwd = getcwd(NULL, 0);
	printf("pwd after chdir: %s\n", pwd);
	return(0);
}

// static char	*find_parent_dir(char *dir)
// {
// 	char	*res;
// 	int		i;
// 	size_t	end;

// 	res = NULL;
// 	i = 0;
// 	while (dir[i] != '\0')
// 		i++;
// 	while (dir[i] != '/')
// 		i--;
// 	end = i;
// 	res = ft_substr(dir, 0, end);
// 	return (res);
// }

// int ft_cd(t_cmd *cmd)
// {
// 	char	*pwd;
// 	char	*home;
// 	char	*new;

// 	home = getenv("HOME");
// 	pwd = getcwd(NULL, 0);
// 	printf("pwd before chdir: %s\n", pwd);
// 	if (cmd->nb_arg > 2)
// 		printf(ERR_ARG, cmd->args[0]);
// 	else if (cmd->nb_arg == 1)
// 		chdir(home); // checker si erreur
// 	else if (cmd->nb_arg == 2)
// 	{
// 		if (ft_strcmp(cmd->args[1], "..") == 0)
// 		{
// 			new = find_parent_dir(pwd);
// 			chdir(new);
// 			free(new);
// 		}
// 	}
// 	pwd = getcwd(NULL, 0);
// 	printf("pwd apres chdir: %s\n", pwd);
// 	return(0);
// }
