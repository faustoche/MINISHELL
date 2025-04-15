/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:47:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 21:32:04 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_file_input_redirection(t_cmd *cmd, int original_stdin)
{
	int	fd_in;

	fd_in = open_file(cmd->in, REDIR_IN, cmd->exit_status);
	if (fd_in == -1)
	{
		close(original_stdin);
		*(cmd->exit_status) = 1;
		return (-1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		close(fd_in);
		close(original_stdin);
		return (-1);
	}
	close(fd_in);
	return (0);
}

int	handle_input_redirection(t_cmd *cmd)
{
	int	original_stdin;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (-1);
	if (cmd->heredoc != -1)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			close(original_stdin);
			*(cmd->exit_status) = 1;
			return (-1);
		}
	}
	else if (cmd->in)
	{
		if (handle_file_input_redirection(cmd, original_stdin) == -1)
		{
			*(cmd->exit_status) = 1;
			return (-1);
		}
	}
	return (original_stdin);
}

int	handle_output_redirection(t_cmd *cmd)
{
	int	fd_out;
	int	original_stdout;

	fd_out = STDOUT_FILENO;
	original_stdout = dup(STDOUT_FILENO);
	if (cmd->out)
	{
		if (cmd->append)
			fd_out = open_file(cmd->out, REDIR_APPEND, cmd->exit_status);
		else
			fd_out = open_file(cmd->out, REDIR_OUT, cmd->exit_status);
		if (fd_out == -1)
		{
			*(cmd->exit_status) = 1;
			return (-1);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	return (original_stdout);
}

static int	prepare_builtin_redir(t_cmd *cmd, int *og_in, int *og_out, int *fd)
{
	*fd = -1;
	if (cmd->heredoc != -1)
		*fd = cmd->heredoc;
	*og_in = handle_input_redirection(cmd);
	if (*og_in == -1)
	{
		*(cmd->exit_status) = 1;
		return (-1);
	}
	*og_out = handle_output_redirection(cmd);
	if (*og_out == -1)
	{
		*(cmd->exit_status) = 1;
		dup2(*og_in, STDIN_FILENO);
		close(*og_in);
		return (-1);
	}
	return (0);
}

void	handle_builtin_redirection(t_cmd *cmd, t_env **env_list)
{
	int	og_stdin;
	int	og_stdout;
	int	heredoc_fd;

	heredoc_fd = -1;
	if (prepare_builtin_redir(cmd, &og_stdin, &og_stdout, &heredoc_fd))
		return ;
	builtins_execution(cmd, env_list);
	dup2(og_stdin, STDIN_FILENO);
	dup2(og_stdout, STDOUT_FILENO);
	close(og_stdin);
	close(og_stdout);
	*(cmd->exit_status) = 0;
	if (heredoc_fd != -1)
	{
		close(heredoc_fd);
		cmd->heredoc = -1;
	}
}
