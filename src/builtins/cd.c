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

int ft_cd(t_cmd *cmd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	printf("pwd before chdir: %s\n", pwd);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		chdir("/home/ghieong/minitalk");
	pwd = getcwd(NULL, 0);
	printf("pwd apres chdir: %s\n", pwd);
	// récupérer la commande cd
	// chercher la variable HOME
	// sauvegarder l'ancien répertoire (OLDPWD)
	// exécuter cd et gérer les erreurs
	// mettre à jour OLDPWD si cd a reussi
	// recupérer le nouveau repertoire (PWD) et l'enregistrer
	// nettoyer la mémoire et retourner le statut
	return(0);
}
