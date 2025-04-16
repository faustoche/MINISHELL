/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:14:49 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 13:48:53 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_redirection(t_cmd *current, int input_fd)
{
	int	fd;

	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (current->in)
	{
		fd = open_file(current->in, REDIR_IN, current->exit_status);
		if (fd != -1)
			dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (current->heredoc != -1)
	{
		dup2(current->heredoc, STDIN_FILENO);
		close(current->heredoc);
	}
}

void	out_redirection(t_cmd *current, t_pipe *pipe_data)
{
	int	fd;

	if (current->next)
	{
		close(pipe_data->pipe_fd[0]);
		dup2(pipe_data->pipe_fd[1], STDOUT_FILENO);
		close(pipe_data->pipe_fd[1]);
	}
	if (current->out)
	{
		if (current->append)
			fd = open_file(current->out, REDIR_APPEND, current->exit_status);
		else
			fd = open_file(current->out, REDIR_OUT, current->exit_status);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

static long long	pipe_exit_code(long long exit_code)
{
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

static long long	pipe_exit_arguments(t_cmd *cmd)
{
	long long	exit_code;
	int			error;

	exit_code = 0;
	error = 0;
	if (cmd->args[1])
	{
		error = parse_exit_code(cmd->args[1], &exit_code);
		if (error == 2)
		{
			exit_error_message(error, cmd->args[1]);
			*(cmd->exit_status) = 2;
			exit(2);
		}
		else if (cmd->args[2])
		{
			exit_error_message(1, NULL);
			*(cmd->exit_status) = 1;
			return (-1);
		}
		else
			return (pipe_exit_code(exit_code));
	}
	return (*(cmd->exit_status));
}

void	pipe_child_process(t_cmd *current, t_pipe *pp)
{
	int	code;

	handle_signals(SIGINT, DEFAULT);
	handle_signals(SIGQUIT, DEFAULT);
	in_redirection(current, pp->input_fd);
	out_redirection(current, pp);
	if (current && current->args && current->args[0])
	{
		if (is_builtins(current->args[0]))
		{
			if (ft_strcmp(current->args[0], "exit") == 0)
			{
				code = pipe_exit_arguments(current);
				if (code != -1)
					(free_pipe(pp->cmd, *(pp->env_list), NULL), exit(code));
			}
			else
				pipe_builtin(current, pp);
		}
		else
			pipe_execve(current, pp);
	}
	else
		free_pipe(pp->cmd, *(pp->env_list), NULL);
	exit(1);
}
