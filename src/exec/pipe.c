/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/06 09:22:03 by fcrocq           ###   ########.fr       */
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
