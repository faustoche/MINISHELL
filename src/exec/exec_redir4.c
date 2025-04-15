/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:48:26 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 18:29:34 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(t_cmd *cmd, t_env *env, int pipefd[2], int fd)
{
	int	code;

	if (fd != -1)
		redir_heredoc(fd);
	else if (cmd->in)
		redir_input(cmd->in, cmd->exit_status);
	if (cmd->out)
		redir_output(cmd->out, cmd->append, cmd->exit_status);
	(close(pipefd[0]), close(pipefd[1]));
	if (cmd->args && cmd->args[0])
	{
		code = redir_execute(cmd, env);
		exit (code);
	}
	exit(EXIT_FAILURE);
}

void	handle_exit_status_signals(int status, t_cmd *cmd)
{
	if (WIFEXITED(status))
		*(cmd->exit_status) = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*(cmd->exit_status) = 128 + WTERMSIG(status);
}

static void	init_original_std(int *og_stdin, int *og_stdout)
{
	*og_stdin = -1;
	*og_stdout = -1;
}

int	apply_redirection(t_cmd *cmd, int *og_stdin, int *og_stdout)
{
	init_original_std(og_stdin, og_stdout);
	if (cmd->in || cmd->heredoc != -1)
		*og_stdin = handle_input_redirection(cmd);
	if (cmd->out)
		*og_stdout = handle_output_redirection(cmd);
	if (*og_stdin == -1 || *og_stdout == -1)
		return (1);
	return (0);
}

int	execute_only_redirections(t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;

	if (apply_redirection(cmd, &original_stdin, &original_stdout))
	{
		*(cmd->exit_status) = 1;
		if (original_stdin != -1)
			(dup2(original_stdin, STDIN_FILENO), close(original_stdin));
		if (original_stdout != -1)
			(dup2(original_stdout, STDOUT_FILENO), close(original_stdout));
		return (1);
	}
	if (original_stdin != -1)
		(dup2(original_stdin, STDIN_FILENO), close(original_stdin));
	if (original_stdout != -1)
		(dup2(original_stdout, STDOUT_FILENO), close(original_stdout));
	if (cmd->heredoc != -1)
	{
		close(cmd->heredoc);
		cmd->heredoc = -1;
	}
	*(cmd->exit_status) = 0;
	return (0);
}
