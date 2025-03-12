/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/11 16:30:51 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// DEFINIR NB CMDS ET VOIR POUR LE CMD
// VOIR L'APPEL A LA FONCTION 

// int	handle_pipe(t_cmd *cmd, int	i, int old_fd)
// {
// 	int	pid;
// 	int	pipefd[2];

// 	if (i >= nb_cmds)
// 		return (0);
// 	if (i != nb_cmds - 1)
// 		pipe(pipefd);
// 	else
// 	{
// 		pipefd[0] = -1;
// 		pipefd[1] = -1;
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (old_fd != -1)
// 		{
// 			dup2(old_fd, STDIN_FILENO);
// 			close (old_fd);
// 		}
// 		if (i != nb_cmds - 1)
// 		{
// 			close(pipefd[0]);
// 			dup2(pipefd[1], STDOUT_FILENO);
// 		}
// 		close(pipefd[1]);
// 		// execute command
// 		return (0);
// 	}
// 	else
// 	{
// 		close(pipefd[1]);
// 		if (old_fd != -1)
// 			close(old_fd);
// 		handle_pipe(cmd, i + 1, pipefd[0]);
// 	}
// 	if (i == 0)
// 		waitpid(pid, NULL, 0);
// 	return (-1);
// }
