/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:05:00 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/12 15:50:04 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

static t_pipe	init_pipe_struct(t_cmd *cmd, t_env **env_list)
{
	t_pipe	pipe_data;

	pipe_data.cmd = cmd;
	pipe_data.env_list = env_list;
	pipe_data.input_fd = STDIN_FILENO;
	pipe_data.pipe_fd[0] = -1;
	pipe_data.pipe_fd[1] = -1;
	return (pipe_data);
}

static void	pipe_child_process(t_cmd *current, t_pipe *pipe_data)
{
	if (handle_signals(SIGINT, DEFAULT) == -1)
		return ;
	if (handle_signals(SIGQUIT, DEFAULT) == -1)
		return ;
	in_redirection(current, pipe_data->input_fd);
	out_redirection(current, pipe_data);
	if (current->args)
	{
		if (is_builtins(current->args[0]))
			pipe_builtin(current, pipe_data);
		else
			pipe_execve(current, pipe_data);
	}
	else
		free_pipe(pipe_data->cmd, *(pipe_data->env_list), NULL);
	exit(1);
}

static void	pipe_parent_process(t_cmd **current, t_pipe *pipe_data)
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

static void cleanup_and_wait(t_pipe *pipe_data)
{
	int     status;
	pid_t   wait_pid;
	t_cmd   *last_cmd;
	int     last_status = 0;

	last_cmd = get_last_cmd(pipe_data->cmd);
	pid_t   last_pid = (last_cmd) ? last_cmd->pid : -1;
	status = 0;
	if (pipe_data->input_fd != STDIN_FILENO)
		close(pipe_data->input_fd);
	while ((wait_pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		if (wait_pid == last_pid)
		{
			if (last_cmd && last_cmd->exit_status)
				*(last_cmd->exit_status) = last_status;
		}
	}
	if (last_cmd && last_cmd->exit_status)
		printf("DEBUG: Code de sortie final: %d\n", *(last_cmd->exit_status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			printf("Quit (core dumped)\n");
	}
	close_all_fd(3);
}

void	execute_pipeline(t_cmd *cmd, t_env *env_list)
{
	t_cmd	*current;
	pid_t	pid;
	t_pipe	pipe_data;

	pipe_data = init_pipe_struct(cmd, &env_list);
	current = cmd;
	while (current)
	{
		if (current->next && !setup_pipe(&pipe_data))
			return ;
		pid = fork();
		if (pid == -1)
		{
			perror("Fork creation failed\n");
			if (current->next)
				(close(pipe_data.pipe_fd[0]), close(pipe_data.pipe_fd[1]));
			return ;
		}
		else if (pid == 0)
			pipe_child_process(current, &pipe_data);
		else
		{
			current->pid = pid;
			pipe_parent_process(&current, &pipe_data);
		}
	}
	cleanup_and_wait(&pipe_data);
}
