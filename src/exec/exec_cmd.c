/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghieong <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/02/26 15:52:05 by ghieong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execve(const char *pathname, char *const argv[], char *const envp[]);

//argv tableau de str (1er elem nom du prog en general)
//envp tableau de str avec variables d'enviro a passer au prog
//-1 si erreur, sinon pas de retour car remplace prog appelant
