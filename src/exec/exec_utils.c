/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:38:54 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/02 14:55:50 by fcrocq           ###   ########.fr       */
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

void	execute_redirect_pipe(t_cmd *cmd, int pipefd[2], pid_t pid, int *stdin_save, t_env *env_list)
{
	int	fd_out;

	if (pid == 0)
	{
		if (is_builtins(cmd->args[0]))
			exit(EXIT_SUCCESS);
		if (cmd->next)
			handle_pipe_redirect(pipefd, 0, NULL);
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
		char *binary_path = find_binary_path(cmd->args[0]);
        if (!binary_path)
        {
            printf(ERR_CMD, cmd->args[0]);
            exit(127);
        }
        if (execve(binary_path, cmd->args, NULL) == -1)
        {
            perror("execve");
            free(binary_path);
            exit(1);
        }
        
        exit(EXIT_FAILURE);
	}
	else
	{
		if (is_builtins(cmd->args[0]))
		{
			builtins_execution(cmd, &env_list);
			free_commands(cmd);
		}
		if (cmd->next)
		{
			handle_pipe_redirect(pipefd, 1, stdin_save);
			execute_commands(cmd->next, env_list);
			handle_pipe_redirect(pipefd, 2, stdin_save);
		}
		waitpid(pid, NULL, 0);
	}
}

void	handle_pipe_redirect(int pipefd[2], int mode, int *stdin_save)
{
	if (mode == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			perror("dup2 failed (stdout)\n");
		close(pipefd[1]);
	}
	else if (mode == 1)
	{
		close(pipefd[1]);
		*stdin_save = dup(STDIN_FILENO);
		if (*stdin_save == -1)
			perror("dup failed (saving stdin)\n");
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			perror("dup2 failed (stdin)\n");
		close(pipefd[0]);
	}
	else if (mode == 2)
	{
		if (*stdin_save != -1 && dup2(*stdin_save, STDIN_FILENO) == -1)
			perror("dup2 failed (stdin)\n");
		close(*stdin_save);
	}
}
