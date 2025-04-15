/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:05:00 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/15 10:08:23 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_kids(pid_t last_pid, t_cmd *last_cmd)
{
	int		status;
	int		last_status;
	pid_t	wait_pid;

	wait_pid = 1;
	last_status = 0;
	while (wait_pid > 0)
	{
		wait_pid = waitpid(-1, &status, 0);
		if (wait_pid > 0)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			if (wait_pid == last_pid && last_cmd && last_cmd->exit_status)
				*(last_cmd->exit_status) = last_status;
		}
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			printf("Quit (core dumped)\n");
	}
	return (last_status);
}

static void	cleanup_and_wait(t_pipe *pipe_data)
{
	t_cmd	*last_cmd;
	pid_t	last_pid;
	int		last_status;

	last_cmd = get_last_cmd(pipe_data->cmd);
	if (last_cmd)
		last_pid = last_cmd->pid;
	else
		last_pid = -1;
	if (pipe_data->input_fd != STDIN_FILENO)
		close(pipe_data->input_fd);
	last_status = wait_kids(last_pid, last_cmd);
	if (pipe_data->cmd && pipe_data->cmd->exit_status)
		*(pipe_data->cmd->exit_status) = last_status;
	close_all_fd(3);
}

static void	process_execute_pipeline(t_cmd *cmd, t_pipe *pipe)
{
	t_cmd	*current;
	pid_t	pid;

	current = cmd;
	while (current)
	{
		if (current->next && !setup_pipe(pipe))
			return ;
		pid = fork();
		if (pid == -1)
		{
			perror("Fork creation failed\n");
			if (current->next)
				(close(pipe->pipe_fd[0]), close(pipe->pipe_fd[1]));
			return ;
		}
		else if (pid == 0)
			pipe_child_process(current, pipe);
		else
		{
			current->pid = pid;
			pipe_parent_process(&current, pipe);
		}
	}
}

void	execute_pipeline(t_cmd *cmd, t_env *env_list)
{
	t_pipe	pipe_data;

	pipe_data = init_pipe_struct(cmd, &env_list);
	process_execute_pipeline(cmd, &pipe_data);
	cleanup_and_wait(&pipe_data);
}

// static void cleanup_and_wait(t_pipe *pipe_data)
// {
// 	int     status;
// 	pid_t   wait_pid;
// 	t_cmd   *last_cmd;
// 	int     last_status = 0;
// 	last_cmd = get_last_cmd(pipe_data->cmd);
// 	pid_t   last_pid = (last_cmd) ? last_cmd->pid : -1;
// 	status = 0;
// 	if (pipe_data->input_fd != STDIN_FILENO)
// 		close(pipe_data->input_fd);
// 	while ((wait_pid = waitpid(-1, &status, 0)) > 0)
// 	{
// 		if (WIFEXITED(status))
// 			last_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 			last_status = 128 + WTERMSIG(status);
// 		if (wait_pid == last_pid)
// 		{
// 			*(last_cmd->exit_status) = last_status;
// 			if (pipe_data->cmd && pipe_data->cmd->exit_status)
// 				*(pipe_data->cmd->exit_status) = last_status;
// 		}
// 	}
// 	if (WIFSIGNALED(status))
// 	{
// 		if (WTERMSIG(status) == SIGQUIT)
// 			printf("Quit (core dumped)\n");
// 	}
// 	close_all_fd(3);
// }

// void	execute_pipeline(t_cmd *cmd, t_env *env_list)
// {
// 	t_cmd	*current;
// 	pid_t	pid;
// 	t_pipe	pipe_data;

// 	pipe_data = init_pipe_struct(cmd, &env_list);
// 	current = cmd;
// 	while (current)
// 	{
// 		if (current->next && !setup_pipe(&pipe_data))
// 			return ;
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("Fork creation failed\n");
// 			if (current->next)
// 				(close(pipe_data.pipe_fd[0]), close(pipe_data.pipe_fd[1]));
// 			return ;
// 		}
// 		else if (pid == 0)
// 			pipe_child_process(current, &pipe_data);
// 		else
// 		{
// 			current->pid = pid;
// 			pipe_parent_process(&current, &pipe_data);
// 		}
// 	}
// 	cleanup_and_wait(&pipe_data);
// }
