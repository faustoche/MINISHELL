/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/26 11:51:36 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_cmd **current)
{
	t_cmd	*new_cmd;

	if (!(*current))
	{
		printf("oups erreur pipe\n");
		return (-1);
	}
	new_cmd = init_command();
	if (!new_cmd)
		return (-1);
	(*current)->next = new_cmd;
	*current = new_cmd;
	return (0);
}

int	pipe_process(int file1, int pfile[2])
{
	int	process;

	process = fork();
	if (process == 0)
	{
		dup2(file1, STDIN_FILENO);
		dup2(pfile[1], STDOUT_FILENO);
		close (file1);
		close(pfile[0]);
		close (pfile[1]);
	}
	else if (process < 0)
	{
		printf("Error creating the processs\n");
		exit (EXIT_FAILURE);
	}
	return (process);
}
/*
int	syntax_check_pipe()
{
	// if arg[0] == "|" || arg[x] == "|"
		// printf("parse error near '|'\n");
	// if command 1 == arg && command 2 == rg
	//Vérifier que chaque commande séparée par | a bien des arguments
	//Après avoir séparé les commandes, vérifie que chaque t_cmd a bien un args[0] (éviter un pipe vide).
	//Si une commande est vide après un |, afficher une erreur "Invalid null command".
}
*/
/*
Dans parse_commands(), chaque | doit créer un nouveau pipe entre deux commandes.
Exemple de structure de données 
*/

// SI pas de commande, alors retour à la ligne