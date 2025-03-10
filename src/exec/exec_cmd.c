/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/03/08 15:32:19 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execve(const char *pathname, char *const argv[], char *const envp[]);

//argv tableau de str (1er elem nom du prog en general)
//envp tableau de str avec variables d'enviro a passer au prog
//-1 si erreur, sinon pas de retour car remplace prog appelant



void	execute_commands(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->args && current->args[0] && is_builtins(current->args[0]))
			builtins_execution(current);
		else
		{
			if (current->args && current->args[0]) // ici execve a la place du if else
                printf(ERR_CMD, current->args[0]);
            else
                printf("Empty command\n");
		}
		current = current->next;
	}
}