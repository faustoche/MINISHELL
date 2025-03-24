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

static void	print_cwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

void ft_cd(t_cmd *cmd)
{
	char	*home;
	char	*old_pwd;
	char	*new_dir;

	home = getenv("HOME");
	old_pwd = getenv("PWD");

	if (cmd->nb_arg == 1) //"cd"
		new_dir = home;
	else if (cmd->nb_arg == 2) //"cd arg"
	{
		if (ft_strcmp(cmd->args[1], "-") == 0)
		{
			new_dir = getenv("OLDPWD");
			if (new_dir == NULL)
			{
				printf("cd : OLDPWD not set\n");
				return ;
			}
			print_cwd();
		}
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
		return ;
	}
	setenv("OLDPWD", old_pwd, 1);//ici enregistrer OLDPWD
	if (chdir(new_dir) == -1)
	{
		perror("cd");
		return ;
	}
	setenv("PWD", new_dir, 1);// ici enregistrer PWD avec new_dir
	print_cwd();
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
