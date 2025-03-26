/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/26 18:06:12 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Exécute la commande */

void	execute_pipeline_cmd(t_cmd *cmd, t_env *env_list)
{
	char	*pathname;

	if (cmd->args && cmd->args[0])
	{
		if (is_builtins(cmd->args[0]))
		{
			builtins_execution(cmd, &env_list);
			exit(EXIT_SUCCESS);
		}
		else
		{
			pathname = find_binary_path(cmd->args[0]);
			if (!pathname || (execve(pathname, cmd->args, NULL) == -1))
			{
				if (pathname)
					free(pathname);
				printf(ERR_CMD, cmd->args[0]);
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		printf("Error: empty command in pipeline\n");
		exit(EXIT_FAILURE);
	}
}

/* 
Configure la sortie du processus enfant et exécute la commande
Le processus enfant écrit son output dans le pipe
*/

void	execute_child(t_cmd *cmd, int pipefd[2], t_env *env_list)
{
	(void)env_list;
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execute_pipeline_cmd(cmd, env_list);
	exit(EXIT_SUCCESS);
}

/*
Configure l'entrée standard du parent et exécute la commande
Permet d'exécuter en chaîne les commandes
*/

void	execute_parent_pipeline(t_cmd *cmd, int pipefd[2], pid_t pid, t_env *env_list)
{
	int	stdin_save;

	close(pipefd[1]);
	stdin_save = dup(STDIN_FILENO);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execute_pipeline(cmd->next, env_list);
	dup2(stdin_save, STDIN_FILENO);
	close(stdin_save);
	waitpid(pid, NULL, 0);
}

/* Exécute la suite de commande */

void	execute_pipeline(t_cmd *cmd, t_env *env_list)
{
	int		pipefd[2];
	pid_t	pid;

	if (!cmd)
		return ;
	if (!has_pipes(cmd) && is_builtins(cmd->args[0]))
	{
		builtins_execution(cmd, &env_list);
		return ;
	}
	if (!cmd->next)
	{
		execute_commands(cmd, env_list);
		return ;
	}
	if (create_pipe(pipefd) == -1)
		return ;
	pid = create_process();
	if (pid == -1)
	{
		handle_pipe_error(pipefd);
		return ;
	}
	if (pid == 0)
		execute_child(cmd, pipefd, env_list);
	else
		execute_parent_pipeline(cmd, pipefd, pid, env_list);
}
