/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 22:31:37 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/14 09:41:08 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(t_pipe *pipe_data)
{
	if (pipe(pipe_data->pipe_fd) == -1)
	{
		print_error_message("Pipe failed\n");
		return (0);
	}
	return (1);
}

void	pipe_builtin(t_cmd *current, t_pipe *pipe_data)
{
	int	exit_code;

	exit_code = 0;
	if (current->next)
	{
		dup2(pipe_data->pipe_fd[1], STDOUT_FILENO);
		close(pipe_data->pipe_fd[1]);
	}
	builtins_execution(current, pipe_data->env_list);
	if (current->exit_status)
		exit_code = *(current->exit_status);
	free_env_list(pipe_data->env_list);
	free_pipe_redir(pipe_data->cmd);
	exit(exit_code);
}

// est-ce que j'ajoute un if (binary)?

static void	free_pipe_execve(char **env, char *binary, t_pipe *data)
{
	free(binary);
	free_env_array(env);
	free_env_list(data->env_list);
	free_commands(data->cmd);
}

void	pipe_execve(t_cmd *current, t_pipe *pipe_data)
{
	char	**env;
	char	*binary_path;
	int		exit_code;

	exit_code = 127;
	binary_path = find_binary_path(current->args[0], *(pipe_data->env_list));
	env = env_list_to_array(*(pipe_data->env_list));
	if (!binary_path)
	{
		(printf(ERR_CMD, current->args[0]), free_env_array(env));
		(free_env_list(pipe_data->env_list), free_commands(pipe_data->cmd));
		exit(exit_code);
	}
	if (access(binary_path, X_OK) == -1)
	{
		exit_code = 126;
		printf("minislay: %s: Permission denied\n", current->args[0]);
		(free_pipe_execve(env, binary_path, pipe_data), exit(exit_code));
	}
	if (execve(binary_path, current->args, env) == -1)
		(free_pipe_execve(env, binary_path, pipe_data), exit(exit_code));
	exit(1);
}

void	pipe_parent_process(t_cmd **current, t_pipe *pipe_data)
{
	if (handle_signals(SIGINT, CHILD_PROMPT) == -1)
		return ;
	if (pipe_data->input_fd != STDIN_FILENO)
		close(pipe_data->input_fd);
	if ((*current)->next)
	{
		close(pipe_data->pipe_fd[1]);
		pipe_data->input_fd = pipe_data->pipe_fd[0];
	}
	if ((*current)->heredoc != -1)
		close((*current)->heredoc);
	*current = (*current)->next;
}
