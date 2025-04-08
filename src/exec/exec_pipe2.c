/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 22:31:37 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/08 23:04:12 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int setup_pipe(t_pipe *pipe_data)
{
	if (pipe(pipe_data->pipe_fd) == -1)
	{
		print_error_message("Pipe failed\n");
		return (0);
	}
	return (1);
}

void    in_redirection(t_cmd *current, int input_fd)
{
	int fd;

	if (input_fd != STDIN_FILENO)
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
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

void    out_redirection(t_cmd *current, t_pipe *pipe_data)
{
	int fd;

	if (current->next)
	{
		close(pipe_data->pipe_fd[0]);
		dup2(pipe_data->pipe_fd[1], STDOUT_FILENO);
		close(pipe_data->pipe_fd[1]);
	}
	
	if (current->out)
	{
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

void    pipe_builtin(t_cmd *current, t_pipe *pipe_data)
{
	if (current->next)
	{
		dup2(pipe_data->pipe_fd[1], STDOUT_FILENO);
		close(pipe_data->pipe_fd[1]);
	}
	builtins_execution(current, pipe_data->env_list);
	free_env_list(pipe_data->env_list);
	free_pipe_redir(pipe_data->cmd);
	exit(0);
}

void    pipe_execve(t_cmd *current, t_pipe *pipe_data)
{
	char    **env;
	char    *binary_path;

	binary_path = find_binary_path(current->args[0]);
	env = env_list_to_array(*(pipe_data->env_list));
	if (!binary_path)
	{
		printf(ERR_CMD, current->args[0]);
		free_env_array(env);
		free_env_list(pipe_data->env_list);
		free_commands(pipe_data->cmd);
		exit(127);
	}
	if (execve(binary_path, current->args, env) == -1)
	{
		free(binary_path);
		free_env_array(env);
		free_env_list(pipe_data->env_list);
		free_commands(pipe_data->cmd);
		exit(1);
	}
}
