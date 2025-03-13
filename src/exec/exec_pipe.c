/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/13 11:32:45 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_single_command(t_cmd *cmd)
{
	if (!cmd->next)
	{
		execute_commands(cmd);
		return ;
	}
}

void	handle_pipe_error(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

void	output_to_pipe(int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
}

void	execute_pipeline_cmd(t_cmd *cmd)
{
	char	*pathname;

	if (cmd->args && cmd->args[0] && is_builtins(cmd->args[0]))
        builtins_execution(cmd);
    else if (cmd->args && cmd->args[0])
    {
        pathname = find_pathname(cmd->args[0]);
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

int setup_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        perror("pipe error");
        return (-1);
    }
    return (0);
}

pid_t create_process()
{
    pid_t pid = fork();
    if (pid == -1)
        perror("fork error");
    return (pid);
}

void setup_parent_pipe_io(int pipefd[2], int *stdin_save)
{
    close(pipefd[1]);
    *stdin_save = dup(STDIN_FILENO);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

void restore_parent_io(int stdin_save, pid_t pid)
{
    dup2(stdin_save, STDIN_FILENO);
    close(stdin_save);
    waitpid(pid, NULL, 0);
}


void	execute_child(t_cmd *cmd, int pipefd[2])
{
	output_to_pipe(pipefd);
    execute_pipeline_cmd(cmd);
    exit(EXIT_SUCCESS);
}

void execute_parent_pipeline(t_cmd *cmd, int pipefd[2], pid_t pid)
{
    int stdin_save;
    
    setup_parent_pipe_io(pipefd, &stdin_save);
    execute_pipeline(cmd->next);
    restore_parent_io(stdin_save, pid);
}

void execute_pipeline(t_cmd *cmd)
{
    int     pipefd[2];
    pid_t   pid;
    
    if (!cmd)
        return;
    
    handle_single_command(cmd);
    if (!cmd->next)
        return;
        
    if (setup_pipe(pipefd) == -1)
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
