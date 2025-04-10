/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:47:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/10 08:48:22 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_file_input_redirection(t_cmd *cmd, int original_stdin)
{
	int	fd_in;

	fd_in = open_file(cmd->in, REDIR_IN);
	if (fd_in == -1)
	{
		close(original_stdin);
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
			return (-1);
		}
	}
	else if (cmd->in)
	{
		if (handle_file_input_redirection(cmd, original_stdin) == -1)
			return (-1);
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
			fd_out = open_file(cmd->out, REDIR_APPEND);
		else
			fd_out = open_file(cmd->out, REDIR_OUT);
		if (fd_out == -1)
		{
			close(original_stdout);  // Ajoutez cette ligne pour fermer le descripteur
			return (-1);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	return (original_stdout);
}

void	handle_builtin_redirection(t_cmd *cmd, t_env **env_list)
{
	int	original_stdin;
	int	original_stdout;
	int	heredoc_fd;

	heredoc_fd = -1;
	if (cmd->heredoc != -1)
		heredoc_fd = cmd->heredoc;
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
	(dup2(original_stdin, STDIN_FILENO), dup2(original_stdout, STDOUT_FILENO));
	(close(original_stdin), close(original_stdout));
	if (heredoc_fd != -1)
	{
		close(heredoc_fd);
		cmd->heredoc = -1;
	}
}
