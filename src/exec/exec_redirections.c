/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:30:49 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/24 11:46:04 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	create_pipe_and_fork(int pipefd[2])
{
	pid_t	pid;

    if (pipe(pipefd) == -1)
    {
        perror("Error : pipe creation failed\n");
        return (-1);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return (-1);
    }
    return (pid);
}

void execute_redirect_pipe(t_cmd *cmd, int pipefd[2], pid_t pid, int *stdin_save, t_env *env_list)
{
    int fd_out;

    if (pid == 0)  // kid
    {
        if (cmd->next)
            handle_pipe(pipefd, 0, NULL);
        if (cmd->in)
            redirect(open_file(cmd->in, REDIR_IN), STDIN_FILENO);
        else if (cmd->heredoc != -1)
            redirect(cmd->heredoc, STDIN_FILENO);
        if (cmd->out)
        {
            if (cmd->append)
                fd_out = open_file(cmd->out, REDIR_APPEND);
            else
                fd_out = open_file(cmd->out, REDIR_OUT);
            redirect(fd_out, STDOUT_FILENO);  
        }
        execute_pipeline_cmd(cmd, env_list);
        exit(EXIT_FAILURE);
    }
    else  // daron
    {
        if (cmd->next)
        {
            handle_pipe(pipefd, 1, stdin_save);
            execute_commands(cmd->next, env_list);
            handle_pipe(pipefd, 2, stdin_save);
        }
        waitpid(pid, NULL, 0);
    }
}

void execute_redirection(t_cmd *cmd, t_env *env_list)
{
    int		pipefd[2];
    pid_t	pid;
    int		stdin_save = -1;

    if (!cmd)
        return;
    pid = create_pipe_and_fork(pipefd);
    if (pid == -1)
        return ;
    execute_redirect_pipe(cmd, pipefd, pid, &stdin_save, env_list);
}

void	handle_pipe(int pipefd[2], int mode, int *stdin_save)
{
	if (mode == 0) // ecriture
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	else if (mode == 1) // lecture
	{
		close(pipefd[1]);
		*stdin_save = dup(STDIN_FILENO);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	else if (mode == 2) // restauration de l'ancienne commande
	{
		dup2(*stdin_save, STDIN_FILENO);
		close(*stdin_save);
	}
}
