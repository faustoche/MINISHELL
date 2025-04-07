/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:38:54 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/05 14:26:11 by fcrocq           ###   ########.fr       */
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

void execute_redirect_pipe(t_cmd *cmd, int pipefd[2], pid_t pid, t_env *env_list)
{
	if (pid == 0)
	{
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		close(pipefd[0]);
		if (is_builtins(cmd->args[0]))
			builtins_execution(cmd, &env_list);
		else
			execute_commands(cmd, env_list);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		if (cmd->heredoc != -1)
		{
			close(cmd->heredoc);
			cmd->heredoc = -1;
		}
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
