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

static char	*find_parent_dir(char *dir)
{
	char	*res;
	int		i;
	size_t	end;

	res = NULL;
	i = 0;
	while (dir[i] != '\0')
		i++;
	while (dir[i] != '/')
		i--;
	end = i;
	res = ft_substr(dir, 0, end);
	return (res);
}

int ft_cd(t_cmd *cmd)
{
	char	*current_dir;
	char	*home_dir;
	char	*new;

	home_dir = getenv("HOME");
	current_dir = getcwd(NULL, 0);
	printf("current_dir before chdir: %s\n", current_dir);
	if (cmd->nb_arg > 2)
		printf(ERR_ARG, cmd->args[0]);
	else if (cmd->nb_arg == 1)
		chdir(home_dir);
	else if (cmd->nb_arg == 2)
	{
		if (ft_strcmp(cmd->args[1], "..") == 0)
		{
			new = find_parent_dir(current_dir);
			chdir(new);
			free(new);
		}
	}
	current_dir = getcwd(NULL, 0);
	printf("current_dir apres chdir: %s\n", current_dir);
	// récupérer la commande cd
	// chercher la variable HOME
	// sauvegarder l'ancien répertoire (OLDpwd)
	// exécuter cd et gérer les erreurs
	// mettre à jour OLDpwd si cd a reussi
	// recupérer le nouveau repertoire (pwd) et l'enregistrer
	// nettoyer la mémoire et retourner le statut
	return(0);
}
