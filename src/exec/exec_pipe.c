/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/18 11:45:58 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Quand il n'y a qu'une seule commande (echo "hello") */

void	handle_single_command(t_cmd *cmd)
{
	if (!cmd->next)
	{
		execute_commands(cmd);
		return ;
	}
}

/* Fermeture des fd */

void	handle_pipe_error(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

/* Redirige la sortie standard vers le pipe d'écriture */

void	output_to_pipe(int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
}

/* Exécute la commande */

void	execute_pipeline_cmd(t_cmd *cmd)
{
	char	*pathname;

	if (cmd->args && cmd->args[0] && is_builtins(cmd->args[0]))
        builtins_execution(cmd);
    else if (cmd->args && cmd->args[0])
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
    else
    {
        printf("Error: empty command in pipeline\n");
        exit(EXIT_FAILURE);
    }
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

/* 
Configure l'entrée standard du parent pour lire depuis le pipe 
Sauvegarde l'ancienne entrée  et redirige l'entrée vers lecture pipe 
*/

void setup_parent_pipe(int pipefd[2], int *stdin_save)
{
    close(pipefd[1]);
    *stdin_save = dup(STDIN_FILENO);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

/* 
Restaure l'entrée standard et attend la fin de processus enfant
On restaure la copie qu'on a fait, une fois la commande exécuté on restaure
l'entrée pour éviter des comportements inattendus
*/

void restore_parent(int stdin_save, pid_t pid)
{
    dup2(stdin_save, STDIN_FILENO);
    close(stdin_save);
    waitpid(pid, NULL, 0);
}

/* 
Configure la sortie du processus enfant et exécute la commande
Le processus enfant écrit son output dans le pipe
*/

void	execute_child(t_cmd *cmd, int pipefd[2])
{
	output_to_pipe(pipefd);
    execute_pipeline_cmd(cmd);
    exit(EXIT_SUCCESS);
}

/*
Configure l'entrée standard du parent et exécute la commande
Permet d'exécuter en chaîne les commandes
*/


void execute_parent_pipeline(t_cmd *cmd, int pipefd[2], pid_t pid)
{
    int stdin_save;
    
    setup_parent_pipe(pipefd, &stdin_save);
    execute_pipeline(cmd->next);
    restore_parent(stdin_save, pid);
}

/* Exécute la suite de commande */

void execute_pipeline(t_cmd *cmd)
{
    int     pipefd[2];
    pid_t   pid;
    
    if (!cmd)
        return;
    
    handle_single_command(cmd);
    if (!cmd->next)
        return;
        
    if (create_pipe(pipefd) == -1)
        return;
    
    pid = create_process();
    if (pid == -1)
    {
        handle_pipe_error(pipefd);
        return;
    }
    
    if (pid == 0)
        execute_child(cmd, pipefd);
    else
        execute_parent_pipeline(cmd, pipefd, pid);
}

int has_pipes(t_cmd *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}
