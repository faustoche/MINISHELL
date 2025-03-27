/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 20:54:57 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/27 12:21:20 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Fermeture des fd */

void	handle_pipe_error(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
	return ;
}

/* Crée un pipe entre deux processus */

int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe error");
		close(pipefd[2]);
		return (-1);
	}
	return (0);
}

/* Crée un processus enfant avec fork() */

pid_t	create_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork error");
	return (pid);
}

int	has_pipes(t_cmd *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}
