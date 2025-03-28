/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:30:49 by faustoche         #+#    #+#             */
/*   Updated: 2025/03/28 14:22:27 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirection(t_cmd *cmd)
{
	int	fd_in;
	int	original_stdin;

	fd_in = STDIN_FILENO;
	original_stdin = dup(STDIN_FILENO);
	if (cmd->in)
	{
		fd_in = open_file(cmd->in, REDIR_IN);
		if (fd_in == -1)
			return (-1);
		dup2(fd_in, STDIN_FILENO);
		close (fd_in);
	}
	else if (cmd->heredoc != -1)
		dup2(cmd->heredoc, STDIN_FILENO);
	return (original_stdin);
}

static int	handle_output_redirection(t_cmd *cmd)
{
	int	fd_out;
	int	original_stdout;

	fd_out = STDOUT_FILENO;
	original_stdout = dup(STDOUT_FILENO);
	if (cmd->out)
	{
		if (cmd->append)
			fd_out = open_file(cmd->out, REDIR_APPEND);
		else
			fd_out = open(cmd->out, REDIR_OUT);
		if (fd_out == -1)
			return (-1);
		dup2(fd_out, STDOUT_FILENO);
		close (fd_out);
	}
	return (original_stdout);
}

static void	handle_builtin_redirection(t_cmd *cmd, t_env **env_list)
{
	int	original_stdin;
	int	original_stdout;

	check_open_fds();
	original_stdin = handle_input_redirection(cmd);
	if (original_stdin == -1)
		return ;
	original_stdout = handle_output_redirection(cmd);
	if (original_stdout == -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return ;
	}
	builtins_execution(cmd, env_list);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	check_open_fds();
}

static void	handle_pipe_redirection(t_cmd *cmd, t_env *env_list)
{
	int		pipefd[2];
	int		stdin_save;
	pid_t	pid;

	stdin_save = -1;
	pid = create_pipe_and_fork(pipefd);
	if (pid == -1)
		return ;
	execute_redirect_pipe(cmd, pipefd, pid, &stdin_save, env_list);
}

void	execute_redirection(t_cmd *cmd, t_env *env_list)
{
	if (!cmd)
		return ;
	if (is_builtins(cmd->args[0]))
		handle_builtin_redirection(cmd, &env_list);
	else
		handle_pipe_redirection(cmd, env_list);
}
