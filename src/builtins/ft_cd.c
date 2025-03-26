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
static t_env	*change_var_value(t_env *env_list, char *name, char *value)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			printf("current->value BEFORE = %s\n", current->value);
			free(current->value);
			current->value = ft_strdup(value);
			printf("current->value AFTER = %s\n", current->value);
			break ;
		}
		current = current->next;
	}
	return (env_list);
}


static void	print_cwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

t_env	*ft_cd(t_cmd *cmd, t_env *env_list)
{
	char	*home;
	char	*pwd;
	char	*old_pwd;
	char	*new_dir;
	t_env	*copied_env_list;

	copied_env_list = copy_env_list(env_list);
	home = getenv("HOME");
	old_pwd = getenv("PWD");
	pwd = getenv("PWD");
	// printf("old_pwd = %s\n", old_pwd);
	// printf("pwd = %s\n", pwd);

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
				return (NULL);
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
		return (NULL);
	}
	if (chdir(new_dir) == -1)
	{
		perror("cd");
		return (NULL);
	}
	copied_env_list = change_var_value(copied_env_list, "OLDPWD", old_pwd); //ici enregistrer OLDPWD
	//printf("new_dir = %s\n", new_dir);
	pwd = getenv("PWD");
	//printf("pwd = %s\n", pwd);
	copied_env_list = change_var_value(copied_env_list, "PWD", pwd);// ici enregistrer PWD avec new_dir
	print_cwd(); //PRINT!!!!!!!1
	return (copied_env_list);
}
