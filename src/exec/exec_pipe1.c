/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:05:00 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/16 16:10:19 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_vars(int *stat, int *last, pid_t *wait_pid, int *print_signal)
{
	*wait_pid = 1;
	*last = 0;
	*print_signal = 0;
	*stat = 0;
}

static int	wait_kids(pid_t last_pid, t_cmd *last_cmd)
{
	int		status;
	int		last_status;
	pid_t	wait_pid;
	int		printed_signal;

	wait_vars(&status, &last_status, &wait_pid, &printed_signal);
	while (wait_pid > 0)
	{
		wait_pid = waitpid(-1, &status, 0);
		if (wait_pid > 0)
		{
			if (wait_pid == last_pid)
			{
				if (WIFEXITED(status))
					last_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					last_status = 128 + WTERMSIG(status);
			}
			if (WIFSIGNALED(status))
				printed_signal = handle_termsig(printed_signal, status);
		}
	}
	if (last_cmd && last_cmd->exit_status)
		*(last_cmd->exit_status) = last_status;
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
			{
				(close(pipe->pipe_fd[0]), close(pipe->pipe_fd[1]));
			}
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
