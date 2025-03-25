/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 20:54:57 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/25 18:27:32 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Fermeture des fd */

void	handle_pipe_error(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

/* Crée un pipe entre deux processus */

int create_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        perror("pipe error");
        return (-1);
    }
    return (0);
}

/* Crée un processus enfant avec fork() */

pid_t create_process()
{
    pid_t pid = fork();
    if (pid == -1)
    perror("fork error");
    return (pid);
}

int has_pipes(t_cmd *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}
