/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:18:40 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/15 18:29:07 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_child_process(pid_t pid, int status, int *code, t_env *env)
{
	int		last_status;
	t_cmd	*last_cmd;
	pid_t	last_pid;
	pid_t	wait_pid;

	last_status = 0;
	last_cmd = get_last_cmd(env->cmd);
	if (last_cmd)
		last_pid = last_cmd->pid;
	else
		last_pid = -1;
	handle_signals(SIGINT, IGNORE);
	wait_pid = waitpid(pid, &status, 0);
	handle_signals(SIGINT, PROMPT);
	if (wait_pid > 0)
	{
		last_status = check_exit_or_signal(status, last_status);
		if (code)
			*code = last_status;
		if (wait_pid == last_pid && last_cmd && last_cmd->exit_status)
			*(last_cmd->exit_status) = last_status;
	}
	return (status);
}

static void	prompt_if_signal(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			g_received_signal = SIGQUIT;
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			printf("\n");
			g_received_signal = SIGINT;
		}
	}
}

int	check_exit_or_signal(int status, int last_status)
{
	if (WIFEXITED(status))
		last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		last_status = 128 + WTERMSIG(status);
	return (last_status);
}

void	execute_child_process(char **args, char *binary_path, t_env *env)
{
	char	**env_arr;

	env_arr = env_list_to_array(env);
	if (!binary_path)
	{
		printf(ERR_CMD, args[0]);
		exit(127);
	}
	if (access(binary_path, X_OK) == -1)
	{
		printf(ERR_CMD, args[0]);
		exit(127);
	}
	else if (execve(binary_path, args, env_arr) == -1)
	{
		close_all_fd(3);
		free_env_array(env_arr);
		exit(127);
	}
	free_env_array(env_arr);
	close_all_fd(3);
}

/* Create child process and execute */

void	create_child_process(char **args, char *bin, t_env *env, int *code)
{
	pid_t	pid;
	int		status;
	int		last_status;

	last_status = 0;
	g_received_signal = 0;
	status = 0;
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
	{
		handle_signals(SIGINT, DEFAULT);
		handle_signals(SIGQUIT, DEFAULT);
		execute_child_process(args, bin, env);
	}
	else
	{
		status = wait_child_process(pid, status, code, env);
		prompt_if_signal(status);
	}
	close_all_fd(3);
}
