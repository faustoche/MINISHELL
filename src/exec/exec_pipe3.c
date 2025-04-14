/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:14:49 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 18:06:22 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_redirection(t_cmd *current, int input_fd)
{
	int	fd;

	dprintf(2, "line = %d, file %s\n", __LINE__, __FILE__);	
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (current->in)
	{
		fd = open_file(current->in, REDIR_IN);
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
		dprintf(2, "line = %d, file %s\n", __LINE__, __FILE__);	
		if (current->append)
			fd = open_file(current->out, REDIR_APPEND);
		else
			fd = open_file(current->out, REDIR_OUT);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

t_pipe	init_pipe_struct(t_cmd *cmd, t_env **env_list)
{
	t_pipe	pipe_data;

	pipe_data.cmd = cmd;
	pipe_data.env_list = env_list;
	pipe_data.input_fd = STDIN_FILENO;
	pipe_data.pipe_fd[0] = -1;
	pipe_data.pipe_fd[1] = -1;
	return (pipe_data);
}

void	pipe_child_process(t_cmd *current, t_pipe *pipe_data)
{
	if (handle_signals(SIGINT, DEFAULT) == -1)
		return ;
	if (handle_signals(SIGQUIT, DEFAULT) == -1)
		return ;
	in_redirection(current, pipe_data->input_fd);
	out_redirection(current, pipe_data);
	if (current && current->args && current->args[0])
	{
		if (is_builtins(current->args[0]))
			pipe_builtin(current, pipe_data);
		else
		{
			pipe_execve(current, pipe_data);
		}
	}
	else
		free_pipe(pipe_data->cmd, *(pipe_data->env_list), NULL);
	exit(1);
}

// 			dprintf(2, "line = %d, file %s\n", __LINE__, __FILE__);	